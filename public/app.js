import { animationFiles } from "./animation-manifest.js";
import { loadAnimationHeader } from "./frame-player.js";
import { createMatrixRenderer } from "./matrix-renderer.js";

const speed = document.querySelector("#speed");
const frameOut = document.querySelector("#frameOut");
const toggleRun = document.querySelector("#toggleRun");
const runLabel = document.querySelector("#runLabel");
const codeView = document.querySelector("#codeView");
const tabs = document.querySelectorAll(".tab");
const animationSelect = document.querySelector("#animationSelect");

const renderer = createMatrixRenderer(document.querySelector("#matrix"));

let animations = [];
let animationIndex = 0;
let frameIndex = 0;
let running = true;
let lastTick = 0;
let playAll = true;

function populateAnimationSelect() {
  animationSelect.replaceChildren(new Option("Cycle all", "all"));
  animations.forEach((animation, index) => {
    animationSelect.append(new Option(animation.name, String(index)));
  });
}

function activeAnimation() {
  return animations[animationIndex];
}

function drawCurrentFrame() {
  const animation = activeAnimation();
  const frame = animation.frames[frameIndex];
  renderer.clear();
  renderer.drawGlyphFrame(frame.rows);
  renderer.flush();
  frameOut.value = `${animation.name} - Frame ${frameIndex + 1}/${animation.frames.length}`;
}

function advanceFrame() {
  const animation = activeAnimation();
  frameIndex += 1;

  if (frameIndex < animation.frames.length) return;

  frameIndex = 0;
  if (playAll) {
    animationIndex = (animationIndex + 1) % animations.length;
  }
}

function loop(now) {
  if (running && animations.length > 0) {
    const frame = activeAnimation().frames[frameIndex];
    const speedMultiplier = Number(speed.value) / 100;
    const interval = frame.duration / speedMultiplier;

    if (now - lastTick >= interval) {
      advanceFrame();
      drawCurrentFrame();
      lastTick = now;
    }
  }
  requestAnimationFrame(loop);
}

async function loadCode(path) {
  const response = await fetch(path);
  codeView.textContent = await response.text();
}

async function initAnimations() {
  animations = await Promise.all(animationFiles.map(loadAnimationHeader));
  populateAnimationSelect();
  drawCurrentFrame();
}

toggleRun.addEventListener("click", () => {
  running = !running;
  toggleRun.textContent = running ? "Pause" : "Resume";
  runLabel.textContent = running ? "Running" : "Paused";
});

animationSelect.addEventListener("change", () => {
  playAll = animationSelect.value === "all";
  animationIndex = playAll ? 0 : Number(animationSelect.value);
  frameIndex = 0;
  lastTick = 0;
  drawCurrentFrame();
});

tabs.forEach((tab) => {
  tab.addEventListener("click", () => {
    tabs.forEach((button) => button.classList.remove("active"));
    tab.classList.add("active");
    loadCode(tab.dataset.file);
  });
});

loadCode("/firmware/jar_pet/jar_pet.ino");
initAnimations();
requestAnimationFrame(loop);
