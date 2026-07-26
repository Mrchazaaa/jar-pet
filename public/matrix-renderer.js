import { matrixConfig } from "./matrix-config.js";
import { colorForGlyph } from "./frame-player.js";

export function createMatrixRenderer(root) {
  const width = matrixConfig.width;
  const height = matrixConfig.height;
  const pixelCount = width * height;
  const buffer = Array.from({ length: pixelCount }, () => [0, 0, 0]);

  const pixels = Array.from({ length: pixelCount }, (_, index) => {
    const node = document.createElement("div");
    node.className = "pixel";
    node.setAttribute("aria-label", `LED ${index}`);
    root.append(node);
    return node;
  });

  function xy(x, y) {
    return y * width + x;
  }

  return {
    width,
    height,
    xy,
    clear() {
      for (let index = 0; index < pixelCount; index += 1) {
        buffer[index] = [0, 0, 0];
      }
    },
    setPixel(x, y, color) {
      if (x < 0 || x >= width || y < 0 || y >= height) return;
      buffer[xy(x, y)] = color;
    },
    drawGlyphFrame(rows) {
      rows.forEach((row, y) => {
        [...row].forEach((glyph, x) => {
          this.setPixel(x, y, colorForGlyph(glyph));
        });
      });
    },
    flush() {
      buffer.forEach(([r, g, b], index) => {
        const isHot = r + g + b > 160;
        pixels[index].style.backgroundColor = `rgb(${r}, ${g}, ${b})`;
        pixels[index].style.boxShadow = isHot
          ? `0 0 18px rgba(${r}, ${g}, ${b}, 0.9), inset 0 0 0 1px rgba(255,255,255,0.24)`
          : "inset 0 0 0 1px rgba(255,255,255,0.05)";
        pixels[index].classList.toggle("hot", isHot);
      });
    }
  };
}
