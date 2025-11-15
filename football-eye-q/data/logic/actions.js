const settingsPage = document.getElementById("settings-page");
const patternsPage = document.getElementById("patterns-page");
const nodeCountSpan = document.getElementById("node-count");
const reprovBanner = document.getElementById("reprov-banner");

settingsPage.style.display = "none";
reprovBanner.style.display = "none";

const tableContainer = document.getElementById("table-container");
const table = document.createElement("table");
let activePattern = null;

const patternsBtn = document.getElementById("patterns-btn");
const settingsBtn = document.getElementById("settings-btn");

patternsBtn.addEventListener("click", onPatternsPressed);
settingsBtn.addEventListener("click", onSettingsPressed);

const defaultNumberOfPatternsToDisplay = 8;
let numberOfPatternsDefault = 0;

performBlockingReuqestForPatterns();

function performBlockingReuqestForPatterns() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/patterns", false);
  try {
    xhr.send();
    if (xhr.status === 200) {
      console.log("Received data:", xhr.responseText);
      const responseText = xhr.responseText;
      const match = responseText.match(/\b\d{1,2}\b/);
      const foundNumber = match ? parseInt(match[0], 10) : null;
      if (foundNumber !== null && foundNumber >= 1 && foundNumber <= 16) {
        console.log("Extracted number:", foundNumber);
        numberOfPatternsDefault = foundNumber;
      } else {
        console.log(
          "Number not found in the string or out of range (1 to 16)."
        );
        numberOfPatternsDefault = defaultNumberOfPatternsToDisplay;
      }
    } else {
      console.error("Request failed with status:", xhr.status);
    }
  } catch (error) {
    console.error("Request error:", error);
  }
}

document.addEventListener("DOMContentLoaded", function () {
  const tabs = document.querySelectorAll(".tab");
  const tabContents = document.querySelectorAll(".tab-content");

  tabs.forEach((tab) => {
    tab.addEventListener("click", function () {
      tabs.forEach((t) => t.classList.remove("active"));
      tab.classList.add("active");

      tabContents.forEach((content) => (content.style.display = "none"));
      const targetContentId = tab
        .getAttribute("id")
        .replace("-tab", "-content");
      const targetContent = document.getElementById(targetContentId);
      targetContent.style.display = "block";
    });
  });
});

document
  .getElementById("reprovision-button")
  .addEventListener("click", function () {
    var confirmation = confirm(
      "Are you sure you want to reprovision your device ?"
    );
    if (confirmation) {
      alert("Your device will be restarted in WiFi Manaager Mode.");
      console.log(
        "At this point we should trgger the device to erase credentials as well as deleting them and reseting. "
      );
      settingsPage.style.display = "none";
      patternsPage.style.display = "none";
      patternsBtn.style.display = "none";
      settingsBtn.style.display = "none";
      nodeCountSpan.style.display = "none";
      reprovBanner.style.display = "block";
      const data = {
        reprov: "reprovision",
      };
      fetch("/reprovision", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
      })
        .then((response) => response.text())
        .then((message) => {
          console.log(message);
        })
        .catch((error) => {
          console.error("Error:", error);
        });
    } else {
      alert("Reprovisioning your device has been cancelled.");
    }
  });

function createCell(text) {
  const cell = document.createElement("td");
  cell.textContent = text;
  return cell;
}

function handleSliderToggleChange(slider, rowNumber) {
  const isChecked = slider.checked;
  if (isChecked) {
    if (activePattern !== null && activePattern !== rowNumber) {
      const previousActiveSlider = document.getElementById(
        `pattern${activePattern}`
      );
      previousActiveSlider.checked = false;
    }
    activePattern = rowNumber;
    console.log(`Pattern ${rowNumber} is ON`);
  } else {
    activePattern = null;
    console.log(`Pattern ${rowNumber} is OFF`);
  }
  const data = {
    pattern: rowNumber,
    state: isChecked,
  };
  fetch("/patterns", {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify(data),
  })
    .then((response) => response.text())
    .then((message) => {
      console.log(message);
    })
    .catch((error) => {
      console.error("Error:", error);
    });
}
function createSliderToggleSwitch(rowNumber) {
  const slider = document.createElement("input");
  slider.type = "checkbox";
  slider.className = "slider-toggle";
  slider.id = `pattern${rowNumber}`;
  slider.addEventListener("click", () =>
    handleSliderToggleChange(slider, rowNumber)
  );
  return slider;
}

function createRow(rowNumber) {
  const row = document.createElement("tr");
  row.appendChild(createCell("Pattern " + rowNumber));
  const cell = createCell("");
  cell.appendChild(createSliderToggleSwitch(rowNumber));
  row.appendChild(cell);
  return row;
}

function createPatternsTable() {
  table.style.width = "100%";

  table.classList.add("patterns-page");

  for (let i = 1; i <= numberOfPatternsDefault; i++) {
    const newRow = createRow(i);
    table.appendChild(newRow);
  }

  const tableContainer = document.getElementById("table-container");
  tableContainer.appendChild(table);
}

function initIndexPage() {
  console.log("Loading page initially.");
  createPatternsTable();
}

function onPatternsPressed() {
  settingsPage.style.display = "none";
  patternsPage.style.display = "block";
  console.log("Show Patterns Page :) ");
}

function onSettingsPressed() {
  settingsPage.style.display = "block";
  patternsPage.style.display = "none";
  console.log("Show Settings Page :) ");
}

initIndexPage();

function updateNodeCount(count) {
  nodeCountSpan.textContent = count;
}

let connectedSimulated = 18;

function simulateRandomNodeChanges() {
  const randomNodeCount = connectedSimulated;
  updateNodeCount(randomNodeCount);
  connectedSimulated--;
  if (connectedSimulated < 0) {
    connectedSimulated = 18;
  }
}

setInterval(simulateRandomNodeChanges, 1000);
