var host = "localhost";
var port = "6789";

var chatSocket = new WebSocket("ws://" + host + ":" + port);
var action_fifo = [];

chatSocket.onmessage = function(e) {
  var data = JSON.parse(e.data);


  if (data["from"] === "bip") {
    console.log(data);
    document.getElementById("chat-log").innerHTML += JSON.stringify(data) + "<br>";
    if (data["action"] === "repaint_screen") {

      while ((current_action_str = action_fifo.shift()) !== undefined) {
        current_action = JSON.parse(current_action_str);
        console.log("Running:");
        console.log(current_action);

        if (current_action["action"] === "text_out") {
          write_text(current_action.content, current_action.x, current_action.y);

        } else if (current_action["action"] === "text_out_center") {
          write_text(current_action.content, current_action.x, current_action.y);

        } else if (current_action["action"] === "set_bg_color") {


        } else if (current_action["action"] === "set_fg_color") {

        } else if (current_action["action"] === "fill_screen_bg") {

        } else if (current_action["action"] === "draw_horizontal_line") {
          color = 'black';
          width = '10';
          draw_line(color, width, current_action.x1, current_action.y, current_action.x2, current_action.y);

        } else if (current_action["action"] === "draw_rect") {
          color = 'red';
          lineWidth = '10';
          width = current_action.x2 - current_action.x1;
          height = current_action.y2 - current_action.y1;
          draw_rectangle(color, lineWidth, current_action.x1, current_action.y1, width, height);

        } else if (current_action["action"] === "draw_vertical_line") {
          color = 'black';
          width = '10';
          draw_line(color, width, current_action.x, current_action.y1, current_action.x, current_action.y2);

        } else if (current_action["action"] === "draw_filled_rect") {
          color = 'red';
          width = current_action.x2 - current_action.x1;
          height = current_action.y2 - current_action.y1;
          draw_rectangle(color, current_action.x1, current_action.y1, width, height);

        } else if (current_action["action"] === "draw_filled_rect_bg") {

        }
      }
    } else {
      console.log("Pushing: " + data.action);
      action_fifo.push(JSON.stringify(data));

    }

  }


};

chatSocket.onclose = function(e) {
  console.error('Closed websocket');
};
