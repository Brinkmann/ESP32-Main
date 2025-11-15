// Get all page elements
const sessionPage = document.getElementById("session-page");
const settingsPage = document.getElementById("settings-page");
const patternsPage = document.getElementById("patterns-page");
const nodeCountSpan = document.getElementById("node-count");
const reprovBanner = document.getElementById("reprov-banner");
const tableContainer = document.getElementById("table-container");

// Get all buttons
const patternsBtn = document.getElementById("patterns-btn");
const settingsBtn = document.getElementById("settings-btn");
const sessionSubmitBtn = document.getElementById("session-submit-btn");
const newSessionBtn = document.getElementById("new-session-btn");

// Get session input elements
const sessionCodeInput = document.getElementById("session-code-input");
const sessionErrorText = document.getElementById("session-error");

const table = document.createElement("table");
let activePattern = null;

// --- Event Listeners ---

patternsBtn.addEventListener("click", onPatternsPressed);
settingsBtn.addEventListener("click", onSettingsPressed);
sessionSubmitBtn.addEventListener("click", onSessionSubmit);
newSessionBtn.addEventListener("click", onNewSession);

// --- Initialization ---

function initIndexPage() {
  console.log("Loading page initially.");
  // Show the session page by default, hide the others
  sessionPage.style.display = "block";
  patternsPage.style.display = "none";
  settingsPage.style.display = "none";
  reprovBanner.style.display = "none";

  // Hide the nav buttons until a session is loaded
  patternsBtn.style.display = "none";
  settingsBtn.style.display = "none";
}

// --- Page Navigation ---

function showPage(pageToShow) {
  // Hide all main pages
  sessionPage.style.display = "none";
  patternsPage.style.display = "none";
  settingsPage.style.display = "none";
  
  // Show the requested one
  pageToShow.style.display = "block";
}

function onPatternsPressed() {
  showPage(patternsPage);
  console.log("Show Playlist Page");
}

function onSettingsPressed() {
  showPage(settingsPage);
  console.log("Show Settings Page");
}

function onNewSession() {
  // Stop any active pattern
  if (activePattern !== null) {
      handleSliderToggleChange(document.getElementById(`pattern${activePattern}`), activePattern, false);
  }
  
  // Clear the table
  while (table.rows.length > 0) {
    table.deleteRow(0);
  }
  
  // Show session entry, hide nav buttons
  showPage(sessionPage);
  patternsBtn.style.display = "none";
  settingsBtn.style.display = "none";
  sessionCodeInput.value = "";
  sessionErrorText.textContent = "";
}

// --- Core Session Logic ---

function onSessionSubmit() {
  const code = sessionCodeInput.value;
  
  // Validate the code
  if (code.length < 2 || code.length > 10 || code.length % 2 !== 0 || !/^\d+$/.test(code)) {
    sessionErrorText.textContent = "Error: Code must be 2, 4, 6, 8, or 10 digits.";
    return;
  }
  
  sessionErrorText.textContent = "";
  
  // Parse the code into pattern IDs
  // "012536" -> ["01", "25", "36"]
  const patternIds = code.match(/.{1,2}/g) || [];
  
  // Convert to numbers
  const patternNumbers = patternIds.map(id => parseInt(id, 10));
  
  // Build the table with these patterns
  createPatternsTable(patternNumbers);
  
  // Show the patterns page and nav buttons
  showPage(patternsPage);
  patternsBtn.style.display = "block";
  settingsBtn.style.display = "block";
}

// --- Table and Pattern Logic ---

function createPatternsTable(patternNumbers) {
  // Clear old table content
  while (table.rows.length > 0) {
    table.deleteRow(0);
  }
  table.style.width = "100%";
  table.classList.add("patterns-page");

  // Create a row for each pattern ID
  patternNumbers.forEach(patternId => {
    // Req 2: Discard IDs outside 1-29
    if (patternId < 1 || patternId > 29) {
        console.warn(`Invalid pattern ID ${patternId} discarded.`);
        return;
    }
    const newRow = createRow(patternId);
    table.appendChild(newRow);
  });
  
  if (table.rows.length === 0) {
    // Req 2: All IDs were invalid
    onNewSession(); // Go back to session page
    sessionErrorText.textContent = "Error: Session code contains no valid patterns (1-29).";
    return;
  }

  tableContainer.appendChild(table);
}

function createRow(rowNumber) {
  const row = document.createElement("tr");
  row.appendChild(createCell("Pattern " + rowNumber));
  const cell = createCell("");
  cell.appendChild(createSliderToggleSwitch(rowNumber));
  row.appendChild(cell);
  return row;
}

function createCell(text) {
  const cell = document.createElement("td");
  cell.textContent = text;
  return cell;
}

function createSliderToggleSwitch(rowNumber) {
  const slider = document.createElement("input");
  slider.type = "checkbox";
  slider.className = "slider-toggle";
  slider.id = `pattern${rowNumber}`;
  
  // We pass 'undefined' for 'forceState' so it reads from the slider
  slider.addEventListener("click", () => handleSliderToggleChange(slider, rowNumber, undefined));
  return slider;
}

// forceState is used to turn off a pattern when loading a new session
function handleSliderToggleChange(slider, rowNumber, forceState) {
  // Use the forced state if provided, otherwise read from the slider
  const isChecked = (forceState !== undefined) ? forceState : slider.checked;
  
  // Update the slider's visual state
  slider.checked = isChecked;

  if (isChecked) {
    // A pattern is turned ON
    // If another pattern is already active, turn it OFF first
    if (activePattern !== null && activePattern !== rowNumber) {
      const previousActiveSlider = document.getElementById(`pattern${activePattern}`);
      if (previousActiveSlider) {
          handleSliderToggleChange(previousActiveSlider, activePattern, false); // Force OFF
      }
    }
    // Set the new active pattern
    activePattern = rowNumber;
    console.log(`Pattern ${rowNumber} is ON`);
  } else {
    // A pattern is turned OFF
    if (activePattern === rowNumber) {
      activePattern = null; // No pattern is active now
    }
    console.log(`Pattern ${rowNumber} is OFF`);
  }
  
  // Send the command to the ESP32 backend
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


// --- Settings Page Logic (Unchanged) ---

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


// --- Node Count Simulation (Unchanged) ---

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

// --- START THE APP ---
initIndexPage();