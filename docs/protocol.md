# Protocol

The simulator and the bip device communicate using websockets using the following messages.



| Action               | From | Message (JSON)                                                                      |
|----------------------|------|-------------------------------------------------------------------------------------|
| text_out             | bip  | {"from": "bip", “action”:”text_out”, “content”:TEXT, “x”:X, “y”:Y}                  |
| text_out_center      | bip  | {"from": "bip", “action”:”text_out_center”, “content”:TEXT, “x”:X, “y”:Y}           |
| set_bg_color         | bip  | {"from": "bip", “action”:”set_bg_color”, “color”:TEXT}                              |
| set_fg_color         | bip  | {"from": "bip", “action”:”set_fg_color”, “color”:TEXT}                              |
| fill_screen_bg       | bip  | {"from": "bip", “action”:”fill_screen_bg”}                                          |
| repaint_screen       | bip  | {"from": "bip", “action”:”repaint_screen”}                                          |
| draw_horizontal_line | bip  | {"from": "bip", “action”:”draw_horizontal_line”, “y”:Y, “x1”: X1, “x2”:X2}          |
| draw_rect            | bip  | {"from": "bip", “action”:”draw_rect”, “x1”:X1, “y1”:Y1, “x2”:X2, “y2”:Y2}           |
| draw_vertical_line   | bip  | {"from": "bip", “action”:”draw_vertical_line”, “x”:X, “y1”: Y1, “y2”:Y2}            |
| draw_filled_rect     | bip  | {"from": "bip", “action”:”draw_filled_rect”, “x1”:X1, “y1”:Y1, “x2”:X2, “y2”:Y2}    |
| draw_filled_rect_bg  | bip  | {"from": "bip", “action”:”draw_filled_rect_bg”, “x1”:X1, “y1”:Y1, “x2”:X2, “y2”:Y2} |
|                      |      |                                                                                     |
| click                | sim  | {“from”: “sim”, “action”:”1”, “x”:X, “y”:Y}                                         |
| swipe_up             | sim  | {“from”: “sim”, “action”:”2”}                                                       |
| swipe_down           | sim  | {“from”: “sim”, “action”:”3”}                                                       |
| swipe_left           | sim  | {“from”: “sim”, “action”:”4”}                                                       |
| swipe_right          | sim  | {“from”: “sim”, “action”:”5”}                                                       |
