const renderButtons = function (parent, config) {
  const container = parent.appendChild(document.createElement("div"));
  container.classList.add("buttons");
  config.classes.forEach((c) => container.classList.add(c));
  config.buttons.forEach((b) => {
    let btn;
    if (b.type > 0) {
      btn = container.appendChild(document.createElement("button"));
      btn.dataset.key = b.key;
      btn.dataset.type = b.type;
    } else {
      btn = container.appendChild(document.createElement("span"));
    }
    btn.innerHTML = b.label;
  });
};

setTimeout(() => {
  renderButtons(document.querySelector("section.remote"), numbers);
  renderButtons(document.querySelector("section.remote"), functional);
  renderButtons(document.querySelector("section.remote"), dpad);
  renderButtons(document.querySelector("section.remote"), media);
  renderButtons(document.querySelector("section.remote"), colors);
  const ws = new WebSocket("ws://192.168.178.218:2339/jsonrpc");
  const btnClear = document.querySelector('button[name="clear"]');
  const btnsKey = document.querySelectorAll("button[data-key]");
  const btnsNav = document.querySelectorAll("nav a");
  ws.onopen = (e) => document.body.classList.remove("ws-off");
  ws.onerror = (e) => document.body.classList.add("ws-off");
  ws.onclose = (e) => document.body.classList.add("ws-off");
  if (btnsKey) {
    let blocked = false;
    btnsNav.forEach((b) => {
      b.addEventListener("click", (e) => {
        ws.addEventListener(
          "close",
          () => {
            setTimeout(() => {
              location.href = e.target.href;
            }, 500);
          },
          { once: true }
        );
        ws.close();
      });
    });
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
}, 1000);
