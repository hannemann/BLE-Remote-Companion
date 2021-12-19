setTimeout(() => {
  const ws = new WebSocket("ws://192.168.178.218:2339/jsonrpc");
  const btnClear = document.querySelector('button[name="clear"]');
  const btnsKey = document.querySelectorAll("button[data-key]");
  ws.onopen = (e) => document.body.classList.remove("ws-off");
  ws.onerror = (e) => document.body.classList.add("ws-off");
  ws.onclose = (e) => document.body.classList.add("ws-off");
  if (btnsKey) {
    let blocked = false;
    btnsKey.forEach((b) => {
      b.addEventListener("pointerdown", (e) => {
        if (document.querySelector('[name="learn"]:checked') || blocked) return;
        blocked = true;
        ws.send(
          JSON.stringify({
            method: "keydown",
            params: e.target.dataset,
          })
        );
      });
    });
    btnsKey.forEach((b) => {
      b.addEventListener("pointerup", (e) => {
        ws.addEventListener("message", (e) => (blocked = false), {
          once: true,
        });
        if (document.querySelector('[name="learn"]:checked')) {
          ws.send(
            JSON.stringify({
              method: "learn",
              params: e.target.dataset,
            })
          );
        } else {
          ws.send(
            JSON.stringify({
              method: "keyup",
              params: e.target.dataset,
            })
          );
        }
      });
    });
  }
  if (btnClear) {
    btnClear.addEventListener("pointerup", (e) => {
      if (confirm("Delete Configuration?")) {
        ws.send(JSON.stringify({ method: "clear" }));
      }
    });
  }
}, 500);
