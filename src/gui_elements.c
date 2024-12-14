#include <stdint.h>
#include <raylib.h>

static int button_with_id(uint64_t id, Rectangle boundary,int* active_button_id){
    Vector2 mouse = GetMousePosition();
    int hoverover = CheckCollisionPointRec(mouse, boundary);
    int clicked = 0;
    if (*active_button_id == 0) {
        if (hoverover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *active_button_id = id;
        }
    } else if (*active_button_id == id) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            *active_button_id = 0;
            if (hoverover) clicked = 1;
        }
    }

    return (clicked<<1) | hoverover;
}
