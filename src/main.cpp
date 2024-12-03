#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Table.H>
#include <FL/fl_ask.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Box.H>
#include <cstdlib> // Include for exit()

// Callback for predefined queries
void predefinedQueryCallback(Fl_Widget* widget, void* data) {
    Fl_Choice* choice = static_cast<Fl_Choice*>(widget);
    const char* selected = choice->text();
    fl_message("Predefined Query Selected: %s", selected);
}

// Callback for running a query
void runQueryCallback(Fl_Widget* widget, void* data) {
    Fl_Text_Display* output = static_cast<Fl_Text_Display*>(data);
    Fl_Text_Buffer* buffer = output->buffer();
    buffer->text("Query executed! Displaying results...");
    // Add logic to connect to the database and display query results
}

// Callback for saving results to a file
void saveToFileCallback(Fl_Widget* widget, void* data) {
    const char* filename = fl_file_chooser("Save Results", "*.txt", nullptr);
    if (filename) {
        FILE* file = fopen(filename, "w");
        if (file) {
            fputs("Query results saved to this file.\n", file);
            fclose(file);
            fl_message("Results saved to: %s", filename);
        } else {
            fl_alert("Failed to save the file.");
        }
    }
}

// Callback for clearing input and output
void clearCallback(Fl_Widget* widget, void* data) {
    Fl_Input* input = static_cast<Fl_Input*>(data);
    input->value(""); // Clear input
    fl_message("Cleared all inputs and results.");
}

// Main Function
int main() {
    // Main window
    Fl_Window* window = new Fl_Window(800, 600, "Walmart Sales Query");
    window->color(fl_rgb_color(255, 255, 255)); // White background

    // Menu Bar
    Fl_Menu_Bar* menubar = new Fl_Menu_Bar(0, 0, 800, 25);
    menubar->add("File/Exit", 0, [](Fl_Widget*, void*) { exit(0); });
    menubar->add("Help/About", 0, [](Fl_Widget*, void*) {
        fl_message("Walmart Sales Query Application\nVersion 1.0");
    });

    // Label for predefined queries
    Fl_Box* queryLabel = new Fl_Box(20, 40, 100, 30, "Queries:");
    queryLabel->labelsize(14); // Increase font size
    queryLabel->labelfont(FL_BOLD); // Make text bold
    queryLabel->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);

    // Predefined Query Dropdown
    Fl_Choice* queryDropdown = new Fl_Choice(150, 40, 200, 30);
    queryDropdown->add("Total Sales by Department");
    queryDropdown->add("Top 5 Stores by Sales");
    queryDropdown->add("Sales for a Specific Date");
    queryDropdown->callback(predefinedQueryCallback, nullptr);

    // Label for manual query input
    Fl_Box* inputLabel = new Fl_Box(20, 80, 100, 30, "Query:");
    inputLabel->labelsize(14); // Increase font size
    inputLabel->labelfont(FL_BOLD); // Make text bold
    inputLabel->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);

    // Input Field
    Fl_Input* input = new Fl_Input(150, 80, 500, 30);
    input->labelcolor(fl_rgb_color(0, 113, 206)); // Blue label
    input->textsize(14);

// Output Display
Fl_Text_Buffer* textbuf = new Fl_Text_Buffer();
Fl_Text_Display* output = new Fl_Text_Display(50, 130, 700, 200);
output->buffer(textbuf);
output->textsize(14);
output->textcolor(fl_rgb_color(0, 113, 206)); // Blue text
output->color(FL_WHITE);                      // Set background to white

// Table for displaying query results
Fl_Table* resultTable = new Fl_Table(50, 350, 700, 200, "Results:");
resultTable->rows(5);   // Set rows
resultTable->cols(3);   // Set columns
resultTable->col_header(1);
resultTable->row_header(1);
resultTable->col_header_color(fl_rgb_color(0, 113, 206));  // Blue header
resultTable->row_header_color(fl_rgb_color(0, 113, 206));  // Blue header
resultTable->color(FL_WHITE);                              // White background
resultTable->box(FL_FLAT_BOX);
resultTable->labelsize(14);                                // Set label size
resultTable->align(FL_ALIGN_TOP);                          // Align label
resultTable->end();

    // Run Query Button
    Fl_Button* runButton = new Fl_Button(50, 550, 100, 30, "Run Query");
    runButton->color(fl_rgb_color(0, 113, 206));  // Blue button
    runButton->labelcolor(FL_WHITE);              // White label
    runButton->callback(runQueryCallback, output);

    // Save Results Button
    Fl_Button* saveButton = new Fl_Button(200, 550, 100, 30, "Save Results");
    saveButton->color(fl_rgb_color(255, 193, 32)); // Yellow button
    saveButton->labelcolor(FL_WHITE);              // White label
    saveButton->callback(saveToFileCallback, nullptr);

    // Clear Button
    Fl_Button* clearButton = new Fl_Button(350, 550, 100, 30, "Clear");
    clearButton->color(fl_rgb_color(0, 113, 206)); // Blue button
    clearButton->labelcolor(FL_WHITE);             // White label
    clearButton->callback(clearCallback, input);

    // Show the Window
    window->end();
    window->show();

    return Fl::run();
}
