var canvas = document.getElementById("bipCanvas");
var ctx = canvas.getContext('2d');

function write_text(text, color, x, y) {
  ctx.font = "80px Arial";
  ctx.fillStyle = color;
  ctx.textBaseline = 'top';
  var lineheight = 70;
  var lines = text.split('\n');
  for (var i = 0; i<lines.length; i++)
      ctx.fillText(lines[i], x, y + (i*lineheight) );

}

function write_text_center(text, color, x, y) {
  ctx.font = "80px Arial";
  ctx.fillStyle = color;
  ctx.textAlign = "center";
  ctx.textBaseline = 'top';
  var lineheight = 70;
  var lines = text.split('\n');
  for (var i = 0; i<lines.length; i++)
      ctx.fillText(lines[i], x, y + (i*lineheight) );

}

function draw_rectangle(color, lineWidth, x0, y0, width, height) {
  ctx.beginPath();
  ctx.lineWidth = lineWidth;
  ctx.strokeStyle = color;
  ctx.rect(x0, y0, width, height);
  ctx.stroke();
}


function draw_filled_rectangle(color, x, y, width, height) {
  ctx.fillStyle = color; // color of fill
  ctx.fillRect(x, y, width, height); // create rectangle
}


function draw_line(color, width, x0, y0, x1, y1) {
  ctx.beginPath();
  ctx.moveTo(x0, y0);
  ctx.lineTo(x1, y1);
  ctx.lineWidth = width;
  ctx.strokeStyle = color;
  ctx.stroke();
}
