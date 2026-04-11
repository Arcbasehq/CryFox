/*
 * Copyright (c) 2026, Tim Flynn <trflynn89@ladybird.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibWebView/WebUI.h>

namespace WebView {

class WEBVIEW_API NewTabUI : public WebUI {
    WEB_UI(NewTabUI);

private:
    virtual void register_interfaces() override;

    void load_top_sites(JsonValue const&);
    void load_privacy_stats(JsonValue const&);
    void set_content_filtering(JsonValue const&);
};

}
