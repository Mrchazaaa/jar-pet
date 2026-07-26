import { createServer } from "node:http";
import { readFile } from "node:fs/promises";
import { extname, join, normalize } from "node:path";

const port = Number(process.env.PORT || 4173);
const host = process.env.HOST || "0.0.0.0";
const root = process.cwd();

const types = {
  ".html": "text/html; charset=utf-8",
  ".css": "text/css; charset=utf-8",
  ".js": "text/javascript; charset=utf-8",
  ".json": "application/json; charset=utf-8",
  ".h": "text/plain; charset=utf-8",
  ".ino": "text/plain; charset=utf-8",
  ".txt": "text/plain; charset=utf-8",
  ".md": "text/markdown; charset=utf-8"
};

function resolvePath(url) {
  const parsed = new URL(url, `http://${host}:${port}`);
  const requestPath = parsed.pathname === "/" ? "/public/index.html" : parsed.pathname;
  const safePath = normalize(requestPath).replace(/^(\.\.[/\\])+/, "");
  return join(root, safePath);
}

createServer(async (req, res) => {
  try {
    const filePath = resolvePath(req.url || "/");
    const body = await readFile(filePath);
    res.writeHead(200, {
      "content-type": types[extname(filePath)] || "application/octet-stream"
    });
    res.end(body);
  } catch {
    res.writeHead(404, { "content-type": "text/plain; charset=utf-8" });
    res.end("Not found");
  }
}).listen(port, host, () => {
  console.log(`RGB matrix page running at http://${host}:${port}`);
  console.log("Use this machine's LAN IP to open it from another device.");
});
