const palette = {
  ".": [0, 0, 0],
  R: [255, 0, 0],
  G: [0, 255, 0],
  B: [0, 0, 255],
  Y: [255, 210, 0],
  C: [0, 220, 255],
  M: [255, 0, 220],
  W: [255, 255, 255],
  O: [255, 96, 0],
  P: [160, 80, 255]
};

export function colorForGlyph(glyph) {
  return palette[glyph] || palette["."];
}

export async function loadAnimationHeader(path) {
  const response = await fetch(path);
  const source = await response.text();
  return parseAnimationHeader(source, path);
}

export function parseAnimationHeader(source, path = "animation.h") {
  const nameMatch = source.match(/const\s+Animation\s+(\w+)\s*=/);
  const frameBlocks = [...source.matchAll(/\{\s*(\d+)\s*,\s*\{([\s\S]*?)\}\s*\}/g)];
  const frames = frameBlocks.map((match) => ({
    duration: Number(match[1]),
    rows: [...match[2].matchAll(/"([^"]+)"/g)].map((row) => row[1])
  })).filter((frame) => frame.rows.length > 0);

  if (!frames.length) {
    throw new Error(`No frames found in ${path}`);
  }

  return {
    id: path.split("/").pop().replace(/\W+/g, "-").toLowerCase(),
    name: nameMatch ? splitName(nameMatch[1]) : path.split("/").pop().replace(".h", ""),
    frames
  };
}

function splitName(name) {
  return name.replace(/([a-z])([A-Z])/g, "$1 $2");
}
