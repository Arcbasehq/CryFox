const bookmarksBarToggle = document.querySelector("#show-bookmarks-bar-toggle");

const loadBookmarksBarSetting = settings => {
    if (!bookmarksBarToggle) {
        return;
    }
    bookmarksBarToggle.checked = Boolean(settings.showBookmarksBar);
};

if (bookmarksBarToggle) {
    bookmarksBarToggle.addEventListener("change", () => {
        ladybird.sendMessage("setShowBookmarksBar", bookmarksBarToggle.checked);
    });
}

document.addEventListener("WebUIMessage", event => {
    if (event.detail.name === "loadSettings") {
        loadBookmarksBarSetting(event.detail.data);
    }
});

document.addEventListener("WebUILoaded", () => {
    ladybird.sendMessage("loadCurrentSettings");
});
