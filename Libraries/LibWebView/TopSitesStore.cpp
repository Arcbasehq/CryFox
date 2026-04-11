/*
 * Copyright (c) 2026, Tim Flynn <trflynn89@ladybird.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Enumerate.h>
#include <AK/Find.h>
#include <AK/JsonArray.h>
#include <AK/JsonObject.h>
#include <AK/QuickSort.h>
#include <AK/StdLibExtras.h>
#include <AK/StringBuilder.h>
#include <AK/Time.h>
#include <LibCore/StandardPaths.h>
#include <LibURL/URL.h>
#include <LibWebView/TopSitesStore.h>
#include <LibWebView/Utilities.h>

namespace WebView {

static constexpr auto VERSION_KEY = "version"sv;
static constexpr auto ITEMS_KEY = "items"sv;

static constexpr auto URL_KEY = "url"sv;
static constexpr auto TITLE_KEY = "title"sv;
static constexpr auto FAVICON_KEY = "favicon"sv;
static constexpr auto VISIT_COUNT_KEY = "visitCount"sv;
static constexpr auto LAST_VISITED_KEY = "lastVisited"sv;

static constexpr auto TOP_SITES_VERSION = 1u;
static constexpr size_t TOP_SITES_MAX_ENTRIES = 200;

static Vector<TopSite> parse_items(JsonArray const& array)
{
    Vector<TopSite> sites;
    sites.ensure_capacity(array.size());

    array.for_each([&](JsonValue const& value) {
        if (!value.is_object())
            return;
        auto const& object = value.as_object();

        auto url = object.get_string(URL_KEY);
        if (!url.has_value() || url->is_empty())
            return;

        TopSite site;
        site.url = url.release_value();
        if (auto title = object.get_string(TITLE_KEY); title.has_value())
            site.title = String { *title };
        if (auto favicon = object.get_string(FAVICON_KEY); favicon.has_value())
            site.favicon_base64_png = String { *favicon };
        site.visit_count = object.get_u64(VISIT_COUNT_KEY).value_or(0);
        site.last_visited_ms = object.get_i64(LAST_VISITED_KEY).value_or(0);
        sites.append(move(site));
    });

    return sites;
}

static JsonObject serialize_site(TopSite const& site)
{
    JsonObject object;
    object.set(URL_KEY, site.url);

    if (site.title.has_value())
        object.set(TITLE_KEY, *site.title);
    if (site.favicon_base64_png.has_value())
        object.set(FAVICON_KEY, *site.favicon_base64_png);

    object.set(VISIT_COUNT_KEY, site.visit_count);
    object.set(LAST_VISITED_KEY, site.last_visited_ms);
    return object;
}

TopSitesStore TopSitesStore::create(Badge<Application>)
{
    auto data_directory = ByteString::formatted("{}/CryFox", Core::StandardPaths::config_directory());
    auto top_sites_path = ByteString::formatted("{}/TopSites.json", data_directory);

    TopSitesStore store { move(top_sites_path) };

    auto top_sites_json = read_json_file(store.m_top_sites_path);
    if (top_sites_json.is_error()) {
        warnln("Unable to read CryFox top sites: {}", top_sites_json.error());
        return store;
    }

    if (auto items = top_sites_json.value().get_array(ITEMS_KEY); items.has_value())
        store.m_sites = parse_items(*items);

    return store;
}

TopSitesStore::TopSitesStore(ByteString top_sites_path)
    : m_top_sites_path(move(top_sites_path))
{
}

Optional<String> TopSitesStore::site_key(URL::URL const& url) const
{
    if (url.scheme() == "about"sv || url.scheme() == "data"sv || url.scheme() == "chrome"sv)
        return {};
    if (!url.host().has_value())
        return {};

    StringBuilder builder;
    builder.append(url.scheme());
    builder.append("//"sv);
    builder.append(url.serialized_host());

    if (auto port = url.port(); port.has_value())
        builder.appendff(":{}", *port);

    builder.append("/"sv);
    auto built = builder.to_string();
    if (built.is_error())
        return {};
    return built.release_value();
}

void TopSitesStore::record_visit(URL::URL const& url, Optional<String> title, Optional<String> favicon_base64_png)
{
    auto key = site_key(url);
    if (!key.has_value())
        return;

    auto now = UnixDateTime::now().milliseconds_since_epoch();

    auto existing = find_if(m_sites.begin(), m_sites.end(), [&](auto const& site) {
        return site.url == *key;
    });

    if (existing != m_sites.end()) {
        existing->visit_count = max<u64>(existing->visit_count + 1, 1u);
        existing->last_visited_ms = now;

        if (title.has_value() && !title->is_empty())
            existing->title = move(title);
        if (favicon_base64_png.has_value())
            existing->favicon_base64_png = move(favicon_base64_png);
    } else {
        TopSite site;
        site.url = key.release_value();
        if (title.has_value() && !title->is_empty())
            site.title = move(title);
        if (favicon_base64_png.has_value())
            site.favicon_base64_png = move(favicon_base64_png);
        site.visit_count = 1;
        site.last_visited_ms = now;
        m_sites.append(move(site));
    }

    prune();
    persist();
}

void TopSitesStore::update_title(URL::URL const& url, String title)
{
    auto key = site_key(url);
    if (!key.has_value() || title.is_empty())
        return;

    auto existing = find_if(m_sites.begin(), m_sites.end(), [&](auto const& site) {
        return site.url == *key;
    });

    if (existing == m_sites.end()) {
        TopSite site;
        site.url = key.release_value();
        site.title = move(title);
        site.visit_count = 0;
        site.last_visited_ms = UnixDateTime::now().milliseconds_since_epoch();
        m_sites.append(move(site));
    } else {
        existing->title = move(title);
    }

    persist();
}

void TopSitesStore::update_favicon(URL::URL const& url, String favicon_base64_png)
{
    auto key = site_key(url);
    if (!key.has_value() || favicon_base64_png.is_empty())
        return;

    auto existing = find_if(m_sites.begin(), m_sites.end(), [&](auto const& site) {
        return site.url == *key;
    });

    if (existing == m_sites.end()) {
        TopSite site;
        site.url = key.release_value();
        site.favicon_base64_png = move(favicon_base64_png);
        site.visit_count = 0;
        site.last_visited_ms = UnixDateTime::now().milliseconds_since_epoch();
        m_sites.append(move(site));
    } else {
        existing->favicon_base64_png = move(favicon_base64_png);
    }

    persist();
}

Vector<TopSite> TopSitesStore::top_sites(size_t limit) const
{
    Vector<TopSite> sites = m_sites;
    if (sites.is_empty())
        return sites;

    auto score = [](TopSite const& site) {
        return static_cast<double>(site.visit_count) * 1'000'000.0 + static_cast<double>(site.last_visited_ms);
    };

    quick_sort(sites, [&](auto const& a, auto const& b) {
        return score(a) > score(b);
    });

    if (sites.size() > limit)
        sites.shrink(limit);
    return sites;
}

void TopSitesStore::persist()
{
    JsonArray items;
    items.ensure_capacity(m_sites.size());

    for (auto const& site : m_sites)
        items.must_append(serialize_site(site));

    JsonObject root;
    root.set(VERSION_KEY, TOP_SITES_VERSION);
    root.set(ITEMS_KEY, move(items));

    if (auto result = write_json_file(m_top_sites_path, root); result.is_error())
        warnln("Unable to persist CryFox top sites: {}", result.error());
}

void TopSitesStore::prune()
{
    if (m_sites.size() <= TOP_SITES_MAX_ENTRIES)
        return;

    auto score = [](TopSite const& site) {
        return static_cast<double>(site.visit_count) * 1'000'000.0 + static_cast<double>(site.last_visited_ms);
    };

    quick_sort(m_sites, [&](auto const& a, auto const& b) {
        return score(a) > score(b);
    });

    m_sites.shrink(TOP_SITES_MAX_ENTRIES);
}

}
