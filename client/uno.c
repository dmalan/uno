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
struct card_t {
    uint32_t type;
    uint32_t color;
    int value;
} up_card;

// window size
int WINDOW_HEIGHT = 600;
int WINDOW_WIDTH = 800;

// current cards in the player's hand
struct card_t hand[52];
int hand_size;

// container for elements drawn inside window
GtkWidget* container;

void card_clicked(GtkWidget*, gpointer);
void clear_container(GtkWidget*);
void deal_random_hand(int);
void draw_card(struct card_t*, int, int);
void draw_hand();
int play(struct card_t*);
void remove_card(struct card_t* card);

int main(int argc, char* argv[]) {
    // fire up gtk
    gtk_init(&argc, &argv);

    // create main window
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "CS50 Uno");

    // container for cards
    container = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), container);

    // when x is pressed, quite app
    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // deal a random hand and display it
    deal_random_hand(5);
    draw_hand();

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
    // cast generic gpointer to a card
    struct card_t* card = (struct card_t*)card_data;

    // play the card
    play(card);
}

/**
 * Remove all children from the given container
 *
 */
void clear_container(GtkWidget* c) {
    // get all children in the container
    GList* children = gtk_container_get_children(GTK_CONTAINER(c));

    // iterate over children of container
    for (GList* i = children; i != NULL; i = g_list_next(i))
          gtk_widget_destroy(GTK_WIDGET(i->data));

    // free list
    g_list_free(children);
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
        struct card_t card = { NUMERIC, (rand() % NUM_COLORS), (rand() % 12) + 1 };
        hand[i] = card;
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
 * @param card Card to draw
 * @param x x-position of card
 * @param y y-position of card
 *
 */
void draw_card(struct card_t* card, int x, int y) {
    // construct label for card
    char label[128];
    sprintf(label, "%d\n%d", card->value, card->color);

    // create button representing card
    GtkWidget* button = gtk_button_new_with_label(label);
    gtk_widget_set_size_request(button, CARD_WIDTH, CARD_HEIGHT);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(card_clicked), card);

    // draw card
    gtk_fixed_put(GTK_FIXED(container), button, x, y);
    gtk_widget_show(button);
}

/**
 * Draw the user's hand
 *
 */
void draw_hand() {
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
        draw_card(&hand[i], column * CARD_WIDTH + padding, row * CARD_HEIGHT + TOP_HEIGHT);
        column++;
    }

    // draw the up card
    draw_card(&up_card, (WINDOW_WIDTH / 2 - CARD_WIDTH / 2), 10);
}

/**
 * Remove the given card from the hand
 *
 * @param card Card to remove 
 *
 */
void remove_card(struct card_t* card) {
    int removed = 0;

    // iterate over hand, looking for card
    for (int i = 0; i < hand_size; i++) {
        // if card has already been removed, then sift element back one
        if (removed)
            hand[i - 1] = hand[i];

        // if card matches, then begin sifting cards backward
        if (hand[i].value == card->value && hand[i].color == card->color)
            removed = 1;
    }

    // since we've removed a card
    hand_size--;
}

/**
 * Play the given card
 *
 * @param card Card to play
 * @return False if card cannot be played, true otherwise
 *
 */
int play(struct card_t* card) {
    // if either value or color matches, then move is valid
    if (card->value == up_card.value || card->color == up_card.color) {
        // change the up card
        up_card.type = card->type;
        up_card.color = card->color;
        up_card.value = card->value;

        // clear game so we can remove the card that was played
        clear_container(container);
        remove_card(card);

        // draw the new game state
        draw_hand();

        return 1;
    }

    // illegal move
    return 0;
}
