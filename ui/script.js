document.addEventListener("DOMContentLoaded", function () {
    const items = document.querySelectorAll(".list-item");

    items.forEach(item => {
        const header = item.querySelector(".item-header");
        const content = item.querySelector(".item-content");

        header.addEventListener("click", () => {
            if (content.style.maxHeight) {
                content.style.maxHeight = null;
            } else {
                content.style.maxHeight = content.scrollHeight + "px";
            }
        });
    });
});
