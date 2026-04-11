/*
 * Copyright (c) 2026, Tim Flynn <trflynn89@ladybird.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/JsonArray.h>
#include <AK/JsonObject.h>
#include <AK/StdLibExtras.h>
#include <LibWebView/Application.h>
#include <LibWebView/TopSitesStore.h>
#include <LibWebView/WebUI/NewTabUI.h>

namespace WebView {

void NewTabUI::register_interfaces()
{
    register_interface("loadTopSites"sv, [this](auto const& data) {
        load_top_sites(data);
    });
}

void NewTabUI::load_top_sites(JsonValue const& data)
{
    auto limit = data.get_u64().value_or(8);
    limit = min<u64>(limit, 24);

    JsonArray sites;
    auto top_sites = Application::top_sites_store().top_sites(limit);
    sites.ensure_capacity(top_sites.size());

    for (auto const& site : top_sites) {
        JsonObject entry;
        entry.set("url"sv, site.url);
        if (site.title.has_value())
            entry.set("title"sv, *site.title);
        if (site.favicon_base64_png.has_value())
            entry.set("favicon"sv, *site.favicon_base64_png);
        sites.must_append(move(entry));
    }

    async_send_message("loadTopSites"sv, move(sites));
}

}
