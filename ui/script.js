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
function changePinState(pin, state) {
  const url = `http://192.168.1.138/digitalOutput?pin=${pin}&state=${state}`;

  return fetch(url, {
    method: "POST", // Use POST method to send data
  })
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
}

// Function to initialize the pin using the initializeDigitalPin HTTP endpoint
async function initializePin(pin, mode) {
  const url = `http://192.168.1.138/initializeDigitalPin?pin=${pin}&mode=${mode}`;

  const response = await fetch(url, {
    method: "POST",
  });

  if (!response.ok) {
    throw new Error("Failed to initialize pin");
  }
}

async function addItem() {
  const typeInput = document.querySelector("#addItemSection .type");
  const pinInput = document.querySelector("#addItemSection .pin");

  const type = typeInput.value.trim();
  const firstLetterOfType = type.charAt(0).toLowerCase();
  const pin = pinInput.value.trim();
  const ioType = document.querySelector("#addItemSection .mode-select").value;

  if (type === "" || pin === "" || ioType === "") {
    alert("Please enter valid values for 'digital/analog', 'pin' & IO.");
    return;
  }

  const itemList = document.getElementById("item-list");
  const listItem = document.createElement("li");
  listItem.className = "thin-list-item thin-list-item";

  if (firstLetterOfType === "d") {
    if (ioType === "output") {
      listItem.innerHTML = `
      <img src="O.svg" class="icon" />
      <span>${type}</span>
      <span>${pin}</span>
      <img src="false.svg" class="checkmark-icon toggle-icon" onclick="toggleIcon(this)" /> <!-- Include the checkmark here -->
  `;
      itemList.appendChild(listItem);
    } else alert("digital inputs are not supported yet");
  }

  if (firstLetterOfType === "a") {
    if (ioType === "input") {
      listItem.innerHTML = `
      <img src="I.svg" class="icon Iicon" />
      <span>${type}</span>
      <span>${pin}</span>
      <div class="value">null</div>
      <img src="reload.svg" class="checkmark-icon toggle-icon" onclick="toggleIcon(this)"/>
  `;
      itemList.appendChild(listItem);
    } else alert("analog outputs are not supported yet");
  }

  if (firstLetterOfType === "d") {
    try {
      await initializePin(pin, ioType);
    } catch (error) {
      console.error(error);
      alert("There was an error initializing or changing the pin state.");
    }

    // Clear input fields
    typeInput.value = "";
    pinInput.value = "";
  }
}

async function toggleIcon(imgElement) {
  const listItem = imgElement.closest(".thin-list-item");
  const type = listItem.querySelector("span:nth-of-type(1)").textContent;
  const pin = listItem.querySelector("span:nth-of-type(2)").textContent;

  if (type.charAt(0).toLowerCase() === "d") {
    //this is checked when adding pins so this is done bad and needs refactoring
    //as it shouldnt be checked more than once, this is just a quick fix

    // Handle digital pin state change
    let newState;
    if (imgElement.src.includes("false.svg")) {
      imgElement.src = "true.svg";
      newState = "high";
    } else {
      imgElement.src = "false.svg";
      newState = "low";
    }
    changePinState(pin, newState);
  } else {
    // Handle analog input request
    const typeOfInput = "voltage";
    const url = `http://192.168.1.138/analogInput?pin=${pin}&type=${typeOfInput}&precision=12`;
    try {
      const response = await fetch(url, { method: "GET" });
      const data = await response.text(); // or response.json() if the response is in JSON format
      const approximatedData = parseFloat(data).toFixed(3);
      listItem.querySelector(".value").textContent = approximatedData + "V";
    } catch (error) {
      console.error("Error fetching analog input:", error);
      listItem.querySelector(".value").textContent = "Error";
    }
  }
}
