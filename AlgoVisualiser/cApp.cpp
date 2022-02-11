#include "cApp.h"

wxIMPLEMENT_APP(cApp);

cApp::cApp() {

}

cApp::~cApp() {
}

bool cApp::OnInit() {

	mainFrame = new cMain();
	mainFrame->Show();

	return true;
}
