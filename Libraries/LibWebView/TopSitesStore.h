/*
 * Copyright (c) 2026, Tim Flynn <trflynn89@ladybird.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Badge.h>
#include <AK/Optional.h>
#include <AK/String.h>
#include <AK/Vector.h>
#include <LibURL/URL.h>
#include <LibWebView/Forward.h>

namespace WebView {

struct WEBVIEW_API TopSite {
    String url;
    Optional<String> title;
    Optional<String> favicon_base64_png;
    u64 visit_count { 0 };
    i64 last_visited_ms { 0 };
};

class WEBVIEW_API TopSitesStore {
public:
    static TopSitesStore create(Badge<Application>);

    void record_visit(URL::URL const&, Optional<String> title = {}, Optional<String> favicon_base64_png = {});
    void update_title(URL::URL const&, String title);
    void update_favicon(URL::URL const&, String favicon_base64_png);

    Vector<TopSite> top_sites(size_t limit) const;

private:
    explicit TopSitesStore(ByteString top_sites_path);

    Optional<String> site_key(URL::URL const&) const;
    void persist();
    void prune();

    ByteString m_top_sites_path;
    Vector<TopSite> m_sites;
};

}
