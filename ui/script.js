// document.addEventListener("DOMContentLoaded", function () {
//     const items = document.querySelectorAll(".list-item");

//     items.forEach(item => {
//         const header = item.querySelector(".item-header");
//         const content = item.querySelector(".item-content");

//         header.addEventListener("click", () => {
//             if (content.style.maxHeight) {
//                 content.style.maxHeight = null;
//             } else {
//                 content.style.maxHeight = content.scrollHeight + "px";
//             }
//         });
//     });
// });

//Function to change the state of a digital or analog output
function changePinState(type, x, state) {
  fetch("http://192.168.1.243/" + type[0] + x + "/" + state)
    .then((response) => {
      if (!response.ok) {
        throw new Error("Network response was not ok");
      }
      return response.text(); // You can change this to response.json() if the server returns JSON.
    })
    .then((data) => {
      console.log("Response:", data);
      // You can add further handling here if needed.
    })
    .catch((error) => {
      console.error("Error:", error);
      // Handle errors here, e.g., display an error message to the user.
    });

  return state;
}

changePinState("digital", 13, "off");

// Function to add a new item to the list
function addItem() {
  const typeInput = document.querySelector("#addItemSection .type");
  const pinInput = document.querySelector(
    "#addItemSection input[type='text'][placeholder='pin']"
  );

  const type = typeInput.value.trim();
  const pin = pinInput.value.trim();

  if (type === "" || pin === "") {
    alert("Please enter valid values for 'digital/analog' and 'pin'.");
    return;
  }

  const itemList = document.getElementById("item-list");
  const listItem = document.createElement("li");
  listItem.className = "thin-list-item";
  listItem.innerHTML = `
      <img src="IO.svg" class="icon" />
      <span>${type}</span>
      <span>${pin}</span>
  `;

  itemList.appendChild(listItem);

  // Clear input fields
  typeInput.value = "";
  pinInput.value = "";
}
