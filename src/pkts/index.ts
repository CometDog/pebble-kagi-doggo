import { ClayConstructor } from "../pktslib/Clay";
// @ts-ignore
const Clay: ClayConstructor = require("pebble-clay");
// @ts-ignore
const clayConfig = require("../pkjs/config");
const clay = new Clay(clayConfig, null, { autoHandleEvents: false });

const sendThemeToWatch = (theme: string) => {
  console.log("[PKJS]: Sending theme: " + theme);
  PebbleTS.sendAppMessage({ type: "update_theme", state: "success", theme });
};

Pebble.addEventListener("showConfiguration", () => {
  try {
    Pebble.openURL(clay.generateUrl());
  } catch (error) {
    console.log("[PKJS]: Failed to open Clay configuration", error);
  }
});

Pebble.addEventListener("webviewclosed", (event) => {
  try {
    if (!event || !event.response) return;
    const settings = clay.getSettings(event.response);
    console.log("[PKJS]: Clay settings received:", settings);
    const darkModeKey = 10000;
    const darkModeVal = settings[darkModeKey];
    const theme = darkModeVal ? "dark" : "light";
    sendThemeToWatch(theme);
  } catch (err) {
    console.error("[PKJS]: Error parsing Clay settings", err);
  }
});

Pebble.addEventListener("ready", async () => {
  console.log("[PKJS]: Ready!");
  const settings = localStorage.getItem("clay-settings");
  const theme = settings
    ? JSON.parse(settings)["darkMode"]
      ? "dark"
      : "light"
    : "light";
  sendThemeToWatch(theme);
});
