#include <oci.h>
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
#include <string>  // Include for std::string


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

    // Oracle DB Connection Logic
    OCIEnv* envhp;
    OCIError* errhp;
    OCISvcCtx* svchp;
    OCIServer* srvhp;
    OCISession* authp;

    // Initialize OCI environment
    if (OCIEnvCreate(&envhp, OCI_DEFAULT, nullptr, nullptr, nullptr, nullptr, 0, nullptr) != OCI_SUCCESS) {
        buffer->text("Failed to initialize Oracle environment.");
        return;
    }

    OCIHandleAlloc(envhp, (void**)&errhp, OCI_HTYPE_ERROR, 0, nullptr);

    OCIHandleAlloc(envhp, (void**)&srvhp, OCI_HTYPE_SERVER, 0, nullptr);
    if (OCIServerAttach(srvhp, errhp, (OraText*)"oracle.cise.ufl.edu:1521/orcl", strlen("oracle.cise.ufl.edu:1521/orcl"), OCI_DEFAULT) != OCI_SUCCESS) {
        char errbuf[512];
        sb4 errcode;
        OCIErrorGet(errhp, 1, NULL, &errcode, (OraText*)errbuf, sizeof(errbuf), OCI_HTYPE_ERROR);
        std::string errorMessage = "Failed to attach to Oracle server: ";
        errorMessage += errbuf; // Append the specific error message
        buffer->text(errorMessage.c_str());
        return;
    }

    OCIHandleAlloc(envhp, (void**)&svchp, OCI_HTYPE_SVCCTX, 0, nullptr);
    OCIAttrSet(svchp, OCI_HTYPE_SVCCTX, srvhp, 0, OCI_ATTR_SERVER, errhp);

    OCIHandleAlloc(envhp, (void**)&authp, OCI_HTYPE_SESSION, 0, nullptr);
    OCIAttrSet(authp, OCI_HTYPE_SESSION, (void*)"emolinalinares@ufl.edu", strlen("emolinalinares@ufl.edu"), OCI_ATTR_USERNAME, errhp);
    OCIAttrSet(authp, OCI_HTYPE_SESSION, (void*)"Bv0AtmHX5QWPDxevm0vuAIOl", strlen("Bv0AtmHX5QWPDxevm0vuAIOl"), OCI_ATTR_PASSWORD, errhp);

    if (OCISessionBegin(svchp, errhp, authp, OCI_CRED_RDBMS, OCI_DEFAULT) != OCI_SUCCESS) {
        buffer->text("Failed to start Oracle session.");
        OCIHandleFree(authp, OCI_HTYPE_SESSION);
        OCIHandleFree(svchp, OCI_HTYPE_SVCCTX);
        OCIHandleFree(srvhp, OCI_HTYPE_SERVER);
        OCIHandleFree(errhp, OCI_HTYPE_ERROR);
        OCIHandleFree(envhp, OCI_HTYPE_ENV);
        return;
    }

    OCIAttrSet(svchp, OCI_HTYPE_SVCCTX, authp, 0, OCI_ATTR_SESSION, errhp);

    // Placeholder for query execution
    buffer->text("Connected to Oracle Database successfully!");

    // Cleanup
    OCISessionEnd(svchp, errhp, authp, OCI_DEFAULT);
    OCIServerDetach(srvhp, errhp, OCI_DEFAULT);
    OCIHandleFree(authp, OCI_HTYPE_SESSION);
    OCIHandleFree(svchp, OCI_HTYPE_SVCCTX);
    OCIHandleFree(srvhp, OCI_HTYPE_SERVER);
    OCIHandleFree(errhp, OCI_HTYPE_ERROR);
    OCIHandleFree(envhp, OCI_HTYPE_ENV);
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
