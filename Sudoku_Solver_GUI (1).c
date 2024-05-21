#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>

// Global variables:
#define BOARD_SIZE 9

int board[BOARD_SIZE][BOARD_SIZE];

GtkWidget *entry[BOARD_SIZE][BOARD_SIZE];

void display_board(int board[BOARD_SIZE][BOARD_SIZE])
{
    for (int row = 1; row <= BOARD_SIZE; row++)
    {
        for (int col = 1; col <= BOARD_SIZE; col++)
        {
            if (board[row - 1][col - 1] == 0)
            {
                printf(". ");
            }
            else
            {
                printf("%d ", board[row - 1][col - 1]);
            }
            if (col % 3 == 0)
            {
                printf("| ");
            }
        }
        printf("\n");
        if (row % 3 == 0)
        {
            printf("------+-------+--------\n");
        }
    }
}

void locate_unfilled_spot(int board[BOARD_SIZE][BOARD_SIZE], int empty_spot[2])
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (board[row][col] == 0)
            {
                empty_spot[0] = row;
                empty_spot[1] = col;
                return; // Return as soon as an empty spot is found
            }
        }
    }
    // If no empty cell is found, set emptyspot to [-1, -1]
    empty_spot[0] = -1;
    empty_spot[1] = -1;
}

bool check_row(int board[BOARD_SIZE][BOARD_SIZE], int row, int num)
{
    for (int col = 0; col < BOARD_SIZE; col++)
    {
        if (board[row][col] == num)
        {
            return false;
        }
    }
    return true;
}

bool check_column(int board[BOARD_SIZE][BOARD_SIZE], int col, int num)
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        if (board[row][col] == num)
        {
            return false;
        }
    }
    return true;
}

bool check_subboard(int board[BOARD_SIZE][BOARD_SIZE], int row, int col, int num)
{
    int row_start = (row / 3) * 3;
    int col_start = (col / 3) * 3;
    for (int r = row_start; r < row_start + 3; r++)
    {
        for (int c = col_start; c < col_start + 3; c++)
        {
            if (board[r][c] == num)
            {
                return false;
            }
        }
    }
    return true;
}

bool is_valid_placement(int board[BOARD_SIZE][BOARD_SIZE], int row, int col, int num)
{
    return check_row(board, row, num) && check_column(board, col, num) && check_subboard(board, row, col, num);
}

bool solve_puzzle(int board[BOARD_SIZE][BOARD_SIZE])
{
    // Count variable renamed for clarity

    int attempts;
    printf("attempts number %d\n", attempts);
    attempts = attempts + 1;
    int empty_spot[2];
    locate_unfilled_spot(board, empty_spot);
    int empty_row = empty_spot[0];
    int empty_col = empty_spot[1];
    if (empty_row == -1)
    {
        printf("Solution found after %d iterations: \n\n", attempts);
        return true;
    }
    else
    {
        for (int guess = 1; guess < 10; guess++)
        {
            if (is_valid_placement(board, empty_row, empty_col, guess))
            {
                board[empty_row][empty_col] = guess;
                if (solve_puzzle(board))
                {
                    return true;
                }

                board[empty_row][empty_col] = 0;
            }
        }
    }
    return false;
}
// called when solve button is clicked
void onsolveButtonClicked(GtkWidget *widget, gpointer data)
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            const char *entry_text = gtk_entry_get_text(GTK_ENTRY(entry[row][col]));
            if (entry_text[0] != '\0')
            {
                board[row][col] = atoi(entry_text);
            }
            else
            {
                board[row][col] = 0;
            }
        }
    }
    printf("Input board:\n");
    display_board(board);

    // Call the solve_puzzle  function to get the solution
    if (solve_puzzle(board))
    {
        for (int row = 0; row < BOARD_SIZE; row++)
        {
            for (int col = 0; col < BOARD_SIZE; col++)
            {
                char buffer[2];
                sprintf(buffer, "%d", board[row][col]);
                gtk_entry_set_text(GTK_ENTRY(entry[row][col]), buffer);
            }
        }
        display_board(board);
    }
    else
    {
        printf("No solution found.\n");
    }
}

// called when Reset button is clicked
void onresetButtonClicked(GtkWidget *widget, gpointer data)
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            gtk_entry_set_text(GTK_ENTRY(entry[row][col]), "");
            board[row][col] = 0;
        }
    }
}

int main(int argc, char *argv[])

{
    GtkWidget *window;
    GtkWidget *board_layout;
    GtkWidget *solve_button;
    GtkWidget *reset_button;

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a new window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Sudoku Solver");

    //  The following line of code, closes the app (not just the window opened) from terminal, if you click close
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 20 size distance from the border of outer window.
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    // Create a new gridls. We will later put buttons in it.
    board_layout = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), board_layout);

    // Initialize the Sudoku grid with 0 values
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j] = 0;
        }
    }

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            // Create an entry
            entry[i][j] = gtk_entry_new();
            gtk_entry_set_max_length(GTK_ENTRY(entry[i][j]), 1);
            gtk_grid_attach(GTK_GRID(board_layout), entry[i][j], j, i, 1, 1);
        }
    }

    /*// Create a button with name "Submit"
    solve_button = gtk_button_new_with_label("Solve");
    reset_button = gtk_button_new_with_label("Reset");
    // if the button is click execute the function button_clicked
    g_signal_connect(solve_button, "clicked", G_CALLBACK(button_clicked), entry);
    g_signal_connect(reset_button, "clicked", G_CALLBACK(button_clicked), entry);
    // attach the button to the grid, and specify the location
    gtk_grid_attach(GTK_GRID(board_layout), solve_button, 3, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(board_layout), reset_button, 4, 9, 1, 1);*/

    solve_button = gtk_button_new_with_label("Solve");
    g_signal_connect(solve_button, "clicked", G_CALLBACK(onsolveButtonClicked), NULL);
    gtk_grid_attach(GTK_GRID(board_layout), solve_button, 0, BOARD_SIZE, BOARD_SIZE, 1);

    reset_button = gtk_button_new_with_label("Reset");
    g_signal_connect(reset_button, "clicked", G_CALLBACK(onresetButtonClicked), NULL);
    gtk_grid_attach(GTK_GRID(board_layout), reset_button, 0, BOARD_SIZE + 1, BOARD_SIZE, 1);

    // It is a loop because the window will be shown and it will stay opened.
    // Show all widgets
    gtk_widget_show_all(window);
    // Start the GTK main loop.
    gtk_main();
    // This is hard coding to receive the "grid"

    return 0;
}