// Wait for the DOM to fully load before running the script
document.addEventListener('DOMContentLoaded', function () {
    // Get references to DOM elements by their IDs
    const angleElement = document.getElementById('angle');
    const distanceElement = document.getElementById('distance');
    const rangeStatusElement = document.getElementById('range-status');
    const objectListElement = document.getElementById('object-list');

    const resetServoButton = document.getElementById('reset-servo');
    const resumeScanningButton = document.getElementById('resume-scanning');
    const ledToggleButton = document.getElementById('led-toggle');

    const forwardButton = document.getElementById('forward-button');
    const backwardButton = document.getElementById('backward-button');
    const rightButton = document.getElementById('right-button');
    const leftButton = document.getElementById('left-button');
    const stopButton = document.getElementById('stop-button');
    const slowButton = document.getElementById('slow-button');
    const mediumButton = document.getElementById('medium-button');
    const fastButton = document.getElementById('fast-button');
    const returnToStartButton = document.getElementById('return-to-start-button');

    // Initialize variables to keep track of states
    let ledState = false;
    let selectedObject = null;

    let angle = 0;
    let distance = 0;
    let objects = [];
    let objectsWithin50 = [];

    // Function to fetch and update data from the server
    function updateData() {
        fetch('/data')
            .then(response => response.json())
            .then(data => {
                const { angle, distance } = data;
                angleElement.textContent = angle;
                distanceElement.textContent = distance;
                rangeStatusElement.textContent = distance <= 100 ? "داخل النطاق" : "خارج النطاق";

                if (distance <= 100) {
                    if (!objects.some(obj => obj.angle === angle && obj.distance === distance)) {
                        objects.push({ angle, distance });
                    }
                    if (objects.length > 100) {
                        objects.shift();
                    }
                }
                draw();
            });
    }

    // Function to send a POST request to a specified URL with given data
    function sendPostRequest(url, data) {
        fetch(url, {
            method: 'POST',
            headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
            body: new URLSearchParams(data)
        });
    }

    // Function to send a command to the server
    function sendCommand(command) {
        fetch('/command', {
            method: 'POST',
            headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
            body: new URLSearchParams({ command })
        });
    }

    // Add event listeners to buttons to perform various actions
    resetServoButton.addEventListener('click', () => {
        sendPostRequest('/servo', { angle: 0 });
    });

    resumeScanningButton.addEventListener('click', () => {
        fetch('/resume');
    });

    ledToggleButton.addEventListener('click', () => {
        sendPostRequest('/led', { state: ledState ? 'off' : 'on' });
        ledState = !ledState;
    });

    forwardButton.addEventListener('click', () => {
        sendCommand('goAhead');
    });

    backwardButton.addEventListener('click', () => {
        sendCommand('goBack');
    });

    rightButton.addEventListener('click', () => {
        sendCommand('goRight');
    });

    leftButton.addEventListener('click', () => {
        sendCommand('goLeft');
    });

    stopButton.addEventListener('click', () => {
        sendCommand('stop');
    });

    slowButton.addEventListener('click', () => {
        sendCommand('slow');
    });

    mediumButton.addEventListener('click', () => {
        sendCommand('medium');
    });

    fastButton.addEventListener('click', () => {
        sendCommand('fast');
    });

    returnToStartButton.addEventListener('click', () => {
        sendCommand('returnToStart');
    });

    // Handle keyboard events to control the robot
    document.addEventListener('keydown', (event) => {
        switch (event.key) {
            case 'ArrowUp':
                sendCommand('goAhead');
                break;
            case 'ArrowDown':
                sendCommand('goBack');
                break;
            case 'ArrowLeft':
                sendCommand('goLeft');
                break;
            case 'ArrowRight':
                sendCommand('goRight');
                break;
            case ' ':
                sendCommand('stop');
                break;
            case '1':
                sendCommand('slow');
                break;
            case '2':
                sendCommand('medium');
                break;
            case '3':
                sendCommand('fast');
                break;
            case '0':
                sendCommand('returnToStart');
                break;
            case '`':
                sendPostRequest('/led', { state: ledState ? 'off' : 'on' });
                ledState = !ledState;
                break;
        }
    });

    // Handle clicks on the object list to select and highlight objects
    objectListElement.addEventListener('click', (event) => {
        const objectText = event.target.textContent;
        const objectData = /Filtered Object \d+: Distance (\d+) cm, Angle (\d+)°/.exec(objectText);
        if (objectData) {
            const [_, distance, angle] = objectData.map(Number);
            sendPostRequest('/servo', { angle });
            selectedObject = { angle, distance };
            draw();
        }
    });

    // Periodically update data from the server
    setInterval(updateData, 1000);
});

// Function to send a command to the server
function sendCommand(command) {
    fetch('/' + command)
        .then(response => response.text())
        .then(data => console.log(data));
}

// Initialize variables for drawing radar and objects
let angle = 0;
let distance = 0;
let objects = [];
let objectsWithin50 = [];
let selectedObject = null;

// Define colors for various elements
const radarColor = [98, 245, 31];
const objectColor = [255, 10, 10];
const selectedObjectColor = [0, 0, 255];
const lineColor = [30, 250, 60];
const circle50Color = [0, 0, 255];

// p5.js setup function to initialize the canvas
function setup() {
    let canvas = createCanvas(1200, 900);
    canvas.parent('canvas-container');
    angleMode(DEGREES);
    noFill();
    strokeWeight(1);
}

// p5.js draw function to update the canvas
function draw() {
    background(0, 4);
    drawRadar();
    drawLine();
    drawStoredObjects();
    drawText();
    updateObjectCount();
}

// Function to draw the radar arcs and lines
function drawRadar() {
    push();
    translate(width / 2, height - height * 0.074);

    let arcSpacing = width * 0.1;

    for (let r = arcSpacing; r <= width; r += arcSpacing) {
        if (r === arcSpacing * 5) {
            stroke(circle50Color);
        } else {
            stroke(radarColor);
        }
        arc(0, 0, r * 2, r * 2, 180, 360);
    }

    stroke(radarColor);
    for (let a = 0; a <= 180; a += 30) {
        let x = (width / 2) * cos(a);
        let y = (width / 2) * sin(a);
        line(0, 0, x, -y);
    }
    pop();
}

// Function to draw the line indicating the current angle
function drawLine() {
    push();
    strokeWeight(9);
    stroke(lineColor);
    translate(width / 2, height - height * 0.074);
    line(0, 0, (height - height * 0.12) * cos(angle), -(height - height * 0.12) * sin(angle));
    pop();
}

// Function to draw the stored objects on the radar
function drawStoredObjects() {
    push();
    translate(width / 2, height - height * 0.074);
    textSize(30);
    for (let obj of objects) {
        let pixsDistance = obj.distance * ((height - height * 0.1666) * 0.01);
        if (selectedObject && selectedObject.angle === obj.angle && selectedObject.distance === obj.distance) {
            fill(selectedObjectColor);
            drawPulsingObject(pixsDistance * cos(obj.angle), -pixsDistance * sin(obj.angle));
        } else {
            fill(objectColor);
            text('❌', pixsDistance * cos(obj.angle), -pixsDistance * sin(obj.angle));
        }
    }
    pop();
}

// Function to draw a pulsing effect for the selected object
function drawPulsingEffect(angle, distance) {
    let startTime = millis();
    let pulseDuration = 30000; // 30 seconds

    function drawPulse() {
        let elapsedTime = millis() - startTime;
        if (elapsedTime < pulseDuration) {
            let pulseSize = width * 0.03 + 30 * sin(elapsedTime * 0.01);
            let pixsDistance = distance * ((height - height * 0.1666) * 0.01);
            stroke(selectedObjectColor);
            ellipse(pixsDistance * cos(angle), -pixsDistance * sin(angle), pulseSize, pulseSize);
            requestAnimationFrame(drawPulse);
        }
    }

    drawPulse();
}

// Function to draw text on the radar indicating distances
function drawText() {
    push();
    fill(0);
    noStroke();
    rect(0, height - height * 0.0648, width, height);
    fill(radarColor);
    textSize(10);

    textAlign(CENTER);
    text("10cm", width - width * 0.3854, height - height * 0.0833);
    text("20cm", width - width * 0.281, height - height * 0.0833);
    text("30cm", width - width * 0.177, height - height * 0.0833);
    text("40cm", width - width * 0.0729, height - height * 0.0833);
    text("50cm", width - width * 0.02, height - height * 0.0833);
    text("60cm", width - width * 0.4854, height - height * 0.0833);
    text("70cm", width - width * 0.481, height - height * 0.0833);
    text("80cm", width - width * 0.477, height - height * 0.0833);
    text("90cm", width - width * 0.473, height - height * 0.0833);
    text("100cm", width - width * 0.469, height - height * 0.0833);

    pop();
}

// Function to update the object count within 50 cm and beyond
function updateObjectCount() {
    let within50 = 0;
    let beyond50 = 0;
    objectsWithin50 = [];
    for (let obj of objects) {
        if (obj.distance <= 50) {
            within50++;
            objectsWithin50.push(obj);
        } else {
            beyond50++;
        }
    }
    document.getElementById('within-50').innerText = within50;
    document.getElementById('beyond-50').innerText = beyond50;
    updateObjectDetails();
}

// Function to update the object details list in the DOM
function updateObjectDetails() {
    let objectList = document.getElementById('object-list');
    objectList.innerHTML = '';

    objectsWithin50.forEach((obj, index) => {
        let listItem = document.createElement('li');
        listItem.className = 'object-item';
        listItem.innerText = `Filtered Object ${index + 1}: Angle ${obj.angle}°, Distance ${obj.distance}cm`;

        listItem.addEventListener('click', () => {
            highlightObject(obj);
        });

        objectList.appendChild(listItem);
    });
}

// Function to highlight a selected object and send a request to the server to move the servo
function highlightObject(obj) {
    selectedObject = obj;
    if (obj.distance <= 100) {
        fetch('/servo', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded'
            },
            body: `angle=${obj.angle}`
        }).then(response => {
            if (response.ok) {
                console.log('Servo directed to selected object successfully');
            } else {
                console.error('Failed to direct servo to selected object');
            }
        });
    }
    draw();
}

// Periodically fetch data from the server and update the radar
setInterval(() => {
    fetch('/data')
        .then(response => response.json())
        .then(data => {
            angle = data.angle;
            distance = data.distance;
            document.getElementById("angle").innerText = angle;
            document.getElementById("distance").innerText = distance;
            document.getElementById("range-status").innerText = distance <= 100 ? "In Range" : "Out of Range";

            if (distance <= 100) {
                if (!objects.some(obj => obj.angle === angle && obj.distance === distance)) {
                    objects.push({ angle, distance });
                }
                if (objects.length > 50) {
                    objects.shift();
                }
            }
            draw();
        });
}, 1000);

// Event listener to resume scanning when the resume button is clicked
document.getElementById('resume-scanning').addEventListener('click', () => {
    fetch('/resume')
        .then(response => {
            if (response.ok) {
                console.log('Resumed scanning');
            } else {
                console.error('Failed to resume scanning');
            }
        });
});

// Event listener to reset the servo to its initial position when the reset button is clicked
document.getElementById('reset-servo').addEventListener('click', () => {
    fetch('/servo', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: 'angle=0'
    }).then(response => {
        if (response.ok) {
            console.log('Servo reset successfully');
        } else {
            console.error('Failed to reset servo');
        }
    });
});

// Function to send a command to the server
function sendCommand(command) {
    fetch('/' + command)
        .then(response => response.text())
        .then(data => console.log(data));
}

// Function to update the distance displayed on the radar
function updateDistance() {
    fetch('/distance')
        .then(response => response.text())
        .then(data => {
            document.getElementById('distance').innerText = data;
        });
}

// Periodically update the distance from the server
setInterval(updateDistance, 1000);
