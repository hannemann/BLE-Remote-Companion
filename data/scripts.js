let ws;

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

let buttonsJson = {};
let blocked = false;

const handleWebsocketResults = function (result) {
  const data = JSON.parse(result.data);
  if (data.buttons) {
    renderButtons(document.querySelector("section.remote"), data.buttons);
  }
  if (data.result === "OK") {
    blocked = false;
  }
};

const addEvents = function () {
  const btnClear = document.querySelector('button[name="clear"]');
  const btnsKey = document.querySelectorAll("button[data-key]");
  const btnsNav = document.querySelectorAll("nav a");
  if (btnsKey) {
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
};

const initButtons = function () {
  let i = 0;
  const bi = setInterval(() => {
    console.log(i);
    ws.send(JSON.stringify({ method: "buttons", type: buttons[i] }));
    i++;
    if (i >= buttons.length) {
      clearInterval(bi);
      addEvents();
      document.body.classList.remove("ws-off");
    }
  }, 200);
};

const deactivate = () => document.body.classList.add("ws-off");
const activate = () => document.body.classList.remove("ws-off");

setTimeout(() => {
  ws = new WebSocket("ws://192.168.178.218:2339/jsonrpc");
  ws.addEventListener("error", deactivate);
  ws.addEventListener("close", deactivate);
  ws.addEventListener("message", handleWebsocketResults);
  ws.addEventListener("open", initButtons);
}, 1000);
