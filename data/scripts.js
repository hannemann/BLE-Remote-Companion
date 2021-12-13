var ws = new WebSocket("ws://192.168.178.218:2339/jsonrpc");
var blocked = false;
ws.onopen = (e) => document.body.classList.remove("ws-off");
ws.onerror = (e) => document.body.classList.add("ws-off");
ws.onclose = (e) => document.body.classList.add("ws-off");
document.querySelectorAll("button[data-key]").forEach((b) => {
  b.addEventListener("pointerdown", (e) => {
    if (document.querySelector('[name="learn"]:checked') || blocked) return;
    blocked = true;
    ws.send(
      JSON.stringify({
        method: "keydown",
        params: { key: e.target.dataset.key },
      })
    );
  });
});
document.querySelectorAll("button").forEach((b) => {
  b.addEventListener("pointerup", (e) => {
    ws.addEventListener("message", (e) => (blocked = false), { once: true });
    if (document.querySelector('[name="learn"]:checked')) {
      ws.send(
        JSON.stringify({
          method: "learn",
          params: { key: e.target.dataset.key },
        })
      );
    } else {
      ws.send(
        JSON.stringify({
          method: "keyup",
          params: { key: e.target.dataset.key },
        })
      );
    }
  });
});
document
  .querySelector('button[name="clear"]')
  .addEventListener("pointerup", (e) => {
    ws.send(JSON.stringify({ method: "clear" }));
  });
