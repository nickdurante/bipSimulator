# Protocol

The simulator and the app device communicate using websockets using the following messages.



| Function             | From | Message (JSON)                                                                      |
|----------------------|------|-------------------------------------------------------------------------------------|
| text_out             | app  | {"from": "app", "action":"text_out", "content":TEXT, "x":X, "y":Y}                  |
| text_out_center      | app  | {"from": "app", "action":"text_out_center", "content":TEXT, "x":X, "y":Y}           |
| set_bg_color         | app  | {"from": "app", "action":"set_bg_color", "color": {R, G, B}}                        |
| set_fg_color         | app  | {"from": "app", "action":"set_fg_color", "color":{R, G, B}}                         |
| fill_screen_bg       | app  | {"from": "app", "action":"fill_screen_bg"}                                          |
| repaint_screen       | app  | {"from": "app", "action":"repaint_screen"}                                          |
| repaint_screen_lines | app  | {"from": "app", "action":"repaint_screen_lines", "from": FROM, "to":TO}             |
| draw_horizontal_line | app  | {"from": "app", "action":"draw_horizontal_line", "y":Y, "x1": X1, "x2":X2}          |
| draw_rect            | app  | {"from": "app", "action":"draw_rect", "x1":X1, "y1":Y1, "x2":X2, "y2":Y2}           |
| draw_vertical_line   | app  | {"from": "app", "action":"draw_vertical_line", "x":X, "y1": Y1, "y2":Y2}            |
| draw_filled_rect     | app  | {"from": "app", "action":"draw_filled_rect", "x1":X1, "y1":Y1, "x2":X2, "y2":Y2}    |
| draw_filled_rect_bg  | app  | {"from": "app", "action":"draw_filled_rect_bg", "x1":X1, "y1":Y1, "x2":X2, "y2":Y2} |
| set_update_period    | app  | {"from": "app", "action":"set_redraw_time", "cmd":int_0_1, "time":time_MS          |
|                      |      |                                                                                     |
| click                | sim  | {"from": "sim", "action":"1", "x":X, "y":Y}                                         |
| swipe_up             | sim  | {"from": "sim", "action":"2"}                                                       |
| swipe_down           | sim  | {"from": "sim", "action":"3"}                                                       |
| swipe_left           | sim  | {"from": "sim", "action":"4"}                                                       |
| swipe_right          | sim  | {"from": "sim", "action":"5"}                                                       |
