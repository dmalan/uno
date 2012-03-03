/**
 * Uno
 * Tommy MacWilliam <tmacwilliam@cs.harvard.edu>
 * Rob Bowden <rob@cs.harvard.edu>
 *
 */

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdint.h>

// size of each card
#define CARD_HEIGHT 100
#define CARD_WIDTH 70

// size of top area of gui
#define TOP_HEIGHT 120

// card-related constants
enum card_type { NUMERIC, WILD, NUM_TYPES };
enum card_color { RED, GREEN, BLUE, YELLOW, NUM_COLORS };

// struct representing a single card
struct card {
    uint32_t type;
    uint32_t color;
    int value;
} up_card;

// window size
static int WINDOW_HEIGHT = 600;
static int WINDOW_WIDTH = 800;

// current cards in the player's hand
static struct card hand[52];
static int hand_size;

void card_clicked(GtkWidget*, gpointer);
void deal_random_hand(int);
void draw_card(GtkWidget*, struct card*, int, int);
void draw_hand();

int main(int argc, char* argv[]) {
    // fire up gtk
    gtk_init(&argc, &argv);

    // create main window
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "CS50 Uno");

    // window container
    GtkWidget* container = gtk_vbox_new(TRUE, 1);
    gtk_container_add(GTK_CONTAINER(window), container);

    // container for cards
    GtkWidget* card_container = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(container), card_container);

    // when x is pressed, quite app
    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // deal a random hand and display it
    deal_random_hand(22);
    draw_hand(card_container);

    // enter main loop
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

/**
 * Event handler for a card being clicked
 *
 */
void card_clicked(GtkWidget* card_button, gpointer card_data) {
    struct card* c = (struct card*)card_data;

    if (c->value == up_card.value || c->color == up_card.color)
        g_print("GOOD JOB!!!\n");
    else
        g_print("I can\'t let you do that StarFox\n");
}

/**
 * Deal a random hand with the given length
 *
 * @param length Number of cards in the hand
 *
 */
void deal_random_hand(int length) {
    // seed prng
    srand(time(NULL));

    // create a hand of size length
    for (int i = 0; i < length; i++) {
        // create a card with random values
        struct card c = { NUMERIC, (rand() % NUM_COLORS), (rand() % 12) + 1 };
        hand[i] = c;
    }

    // keep track of size of hand
    hand_size = length;

    // generate random up card
    up_card.type = NUMERIC;
    up_card.color = rand() % NUM_COLORS;
    up_card.value = rand() % 12 + 1;
}

/**
 * Draw a single card
 *
 * @param container Container to draw card within
 * @param c Card to draw
 *
 */
void draw_card(GtkWidget* container, struct card* c, int x, int y) {
    // construct label for card
    char label[128];
    sprintf(label, "%d\n%d", c->value, c->color);

    // create button representing card
    GtkWidget* button = gtk_button_new_with_label(label);
    gtk_widget_set_size_request(button, CARD_WIDTH, CARD_HEIGHT);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(card_clicked), c);

    // draw card
    gtk_fixed_put(GTK_FIXED(container), button, x, y);
}

/**
 * Draw the user's hand
 *
 * @param container Container to draw cards within
 *
 */
void draw_hand(GtkWidget* container) {
    int column = 0;
    int row = 0;
    int padding = 3;
    int cards_per_row = WINDOW_WIDTH / (CARD_WIDTH + padding);

    // draw all cards in the hand
    for (int i = 0; i < hand_size; i++) {
        // if card cannot fit on this row, then move to the next row
        if (column * CARD_WIDTH + padding > WINDOW_WIDTH - CARD_WIDTH - 3) {
            row++;
            column = 0;
        }

        // draw the current card, then move to the next column
        draw_card(container, &hand[i], column * CARD_WIDTH + padding, row * CARD_HEIGHT + TOP_HEIGHT);
        column++;
    }

    // draw the up card
    draw_card(container, &up_card, (WINDOW_WIDTH / 2 - CARD_WIDTH / 2), 10);
}
