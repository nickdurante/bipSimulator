var host = "localhost";
var port = "6789";

var simSocket = new WebSocket("ws://" + host + ":" + port);
var action_fifo = [];
// orange
var bg_color = "#ffb400";
//green
var fg_color = "#6ace96";

var timer;

simSocket.onmessage = function(e) {

  // received message from ws
  var data = JSON.parse(e.data);
  //console.log(data);
  // message from from bip
  if (data["from"] === "app") {

    document.getElementById("chat-log").innerHTML += JSON.stringify(data) + "<br>";
    if (data["action"] === "repaint_screen") {

      while ((current_action_str = action_fifo.shift()) !== undefined) {
        current_action = JSON.parse(current_action_str);
        console.log("Running:");
        console.log(current_action);

        switch (current_action["action"]) {
          case "text_out":
            console.log("Writing: " + current_action.content);
            write_text(current_action.content, fg_color, current_action.x * 4, current_action.y * 4);
            break;
          case "text_out_center":
            //TODO: center text
            console.log("Writing: " + current_action.content);

            write_text(current_action.content, fg_color, current_action.x * 4, current_action.y * 4);
            break;
          case "set_bg_color":
            bg_color = rgbToHex(current_action["color"]);
            console.log("Set bg: " + bg_color);
            break;
          case "set_fg_color":
            fg_color = rgbToHex(current_action["color"]);
            console.log("Set fg: " + fg_color);
            break;
          case "fill_screen_bg":
            console.log("Filling screen with: " + bg_color);
            draw_filled_rectangle(bg_color, 0, 0, 704, 704);
            break;
          case "draw_horizontal_line":
            width = '5';
            console.log("Drawing horizontal line with: " + fg_color);
            draw_line(fg_color, width, current_action.x1 * 4, current_action.y * 4, current_action.x2 * 4, current_action.y * 4);
            break;
          case "draw_rect":
            lineWidth = '5';
            width = (current_action.x2 - current_action.x1) * 4;
            height = (current_action.y2 - current_action.y1) * 4;
            console.log("Drawing rectangle with: " + fg_color);
            draw_rectangle(fg_color, lineWidth, current_action.x1 * 4, current_action.y1 * 4, width, height);
            break;
          case "draw_vertical_line":
            width = '5';
            console.log("Drawing vertical line with: " + fg_color);
            draw_line(fg_color, width, current_action.x * 4, current_action.y1 * 4, current_action.x * 4, current_action.y2 * 4);
            break;

          case "draw_filled_rect":
            width = (current_action.x2 - current_action.x1) * 4;
            height = (current_action.y2 - current_action.y1) * 4;
            console.log("Drawing rectangle with: " + fg_color);
            draw_filled_rectangle(fg_color, current_action.x1 * 4, current_action.y1 * 4, width, height);
            break;
          case "draw_filled_rect_bg":
            width = (current_action.x2 - current_action.x1) * 4;
            height = (current_action.y2 - current_action.y1) * 4;
            console.log("Drawing filled rectangle with: " + bg_color);
            draw_filled_rectangle(bg_color, current_action.x1 * 4, current_action.y1 * 4, width, height);
            break;
          default:
            console.log("Unrecognized option");
        }

      }
    } else if (data["action"] === "set_redraw_time") {
      if (data["cmd"] == 1) {

        console.log("Start timer");
        timer = setTimeout(sendRefresh, parseInt(data["time"]));
      } else {
        console.log("Set timer to false")
        clearTimeout(timer);
      }
    } else {

      // put in the FIFO queue
      action_fifo.push(JSON.stringify(data));

    }
  }

};

function componentToHex(c) {
  var hex = c.toString(16);
  return hex.length == 1 ? "0" + hex : hex;
};

function rgbToHex(color) {
  r = color[0];
  g = color[1];
  b = color[2];
  console.log(color);
  return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
};


function send_button_ws(id) {
  var outString = JSON.stringify({
    'from': 'sim',
    'action': id
  });

  simSocket.send(outString);
  document.getElementById("chat-log").innerHTML += outString + "<br>";

};

function getCursorPosition(canvas, event) {
  const rect = canvas.getBoundingClientRect()
  const x = Math.round((event.clientX - rect.left) / 4);
  const y = Math.round((event.clientY - rect.top) / 4);
  var outString = JSON.stringify({
    "from": "sim",
    "action": 1,
    "x": x,
    "y": y
  });

  simSocket.send(outString);
  document.getElementById("chat-log").innerHTML += outString + "<br>";
}


function sendRefresh() {
  var outString = JSON.stringify({
    'from': 'app',
    'action': 'repaint_screen'
  });
  simSocket.send(outString);
}

canvas.addEventListener('mousedown', function(e) {
  getCursorPosition(canvas, e)
})

simSocket.onclose = function(e) {
  console.error('Closed websocket');
};
