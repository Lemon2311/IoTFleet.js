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
      <img src="false.svg" class="checkmark-icon toggle-icon" onclick="toggleIcon(this)" /> <!-- Include the checkmark here -->
  `;

  itemList.appendChild(listItem);

  changePinState(type, pin, "off");

  // Clear input fields
  typeInput.value = "";
  pinInput.value = "";
}

function toggleIcon(imgElement) {
  // Find the parent list item of the clicked image
  // This is necessary to get the context in which the image was clicked
  const listItem = imgElement.closest('.thin-list-item');

  // Extract the type and pin values from the list item
  // These values are assumed to be in the first and second span elements respectively
  // 'textContent' is used to get the text inside the span elements
  const type = listItem.querySelector('span:nth-of-type(1)').textContent;
  const pin = listItem.querySelector('span:nth-of-type(2)').textContent;

  // Initialize a variable to hold the new state of the pin
  let newState;

  // Check the current source of the image to determine the toggle action
  // If the current image is 'false.svg', switch to 'true.svg' and set the new state to 'on'
  // If the current image is not 'false.svg', switch to 'false.svg' and set the new state to 'off'
  if (imgElement.src.includes('false.svg')) {
      imgElement.src = 'true.svg';
      newState = 'on';
  } else {
      imgElement.src = 'false.svg';
      newState = 'off';
  }

  // Call the changePinState function with the type, pin, and the new state
  // This function is expected to handle the logic of changing the state of the pin
  // The exact behavior of this function depends on its implementation which is not shown here
  changePinState(type, pin, newState);
}
