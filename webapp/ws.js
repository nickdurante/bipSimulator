var host = "localhost";
var port = "6789";

var chatSocket = new WebSocket("ws://" + host + ":" + port);
var action_fifo = [];
// orange
var bg_color = "#ffb400";
//green
var fg_color = "#6ace96";

chatSocket.onmessage = function(e) {
  var data = JSON.parse(e.data);


  if (data["from"] === "app") {
    console.log(data);
    document.getElementById("chat-log").innerHTML += JSON.stringify(data) + "<br>";
    if (data["action"] === "repaint_screen") {

      while ((current_action_str = action_fifo.shift()) !== undefined) {
        current_action = JSON.parse(current_action_str);
        console.log("Running:");
        console.log(current_action);

        if (current_action["action"] === "text_out") {
          write_text(current_action.content, current_action.x * 4, current_action.y * 4);

        } else if (current_action["action"] === "text_out_center") {
          //TODO: center text
          write_text(current_action.content, current_action.x, current_action.y);

        } else if (current_action["action"] === "set_bg_color") {
          bg_color = rgbToHex(current_action["color"]);

        } else if (current_action["action"] === "set_fg_color") {
          fg_color = rgbToHex(current_action["color"]);

        } else if (current_action["action"] === "fill_screen_bg") {
          draw_filled_rectangle(bg_color, 0, 0, 704, 704);

        } else if (current_action["action"] === "draw_horizontal_line") {
          //color = 'black';
          width = '5';
          draw_line(fg_color, width, current_action.x1 * 4, current_action.y * 4, current_action.x2 * 4, current_action.y * 4);

        } else if (current_action["action"] === "draw_rect") {
          //color = 'red';
          lineWidth = '10';
          width = (current_action.x2 - current_action.x1) * 4;
          height = (current_action.y2 - current_action.y1) * 4;
          draw_rectangle(fg_color, lineWidth, current_action.x1 * 4, current_action.y1 * 4, width, height);

        } else if (current_action["action"] === "draw_vertical_line") {
          //color = 'black';
          width = '10';
          draw_line(fg_color, width, current_action.x * 4, current_action.y1 * 4, current_action.x * 4, current_action.y2 * 4);

        } else if (current_action["action"] === "draw_filled_rect") {
          //color = 'red';
          width = (current_action.x2 - current_action.x1) * 4;
          height = (current_action.y2 - current_action.y1) * 4;

          draw_filled_rectangle(fg_color, current_action.x1 * 4, current_action.y1 * 4, width, height);

        } else if (current_action["action"] === "draw_filled_rect_bg") {
          width = (current_action.x2 - current_action.x1) * 4;
          height = (current_action.y2 - current_action.y1) * 4;
          draw_filled_rectangle(bg_color, current_action.x1 * 4, current_action.y1 * 4, width, height);

        }
      }
    } else {
      console.log("Pushing: " + data.action);
      if (data["action"] === "set_bg_color") {
        //put at the beginning of the FIFO
        action_fifo.unshift(JSON.stringify(data));
      } else {
        // put in the FIFO queue
        action_fifo.push(JSON.stringify(data));

      }
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

  return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
};


function send_button_ws(id) {
  var outString = JSON.stringify({
    'from': 'sim',
    'action': id
  });

  chatSocket.send(outString);
  document.getElementById("chat-log").innerHTML += outString + "<br>";

};

function getCursorPosition(canvas, event) {
  const rect = canvas.getBoundingClientRect()
  const x = Math.round((event.clientX - rect.left) / 4);
  const y = Math.round((event.clientY - rect.top) / 4);
  var outString = JSON.stringify({
    "from": "sim",
    "action": "1",
    "x": x,
    "y": y
  });

  chatSocket.send(outString);
  document.getElementById("chat-log").innerHTML += outString + "<br>";
}


canvas.addEventListener('mousedown', function(e) {
  getCursorPosition(canvas, e)
})

chatSocket.onclose = function(e) {
  console.error('Closed websocket');
};
