#include "ofxPUI.h"
#include "testApp.h"

void dump_object_chain (puObject * tptr, const char * name) {
	printf ("Object: %s\n", name);
	while (tptr != NULL) {
		printf("  My Ptr: %d\n", tptr);
		printf("  My Type: %s\n", tptr->getTypeString());
		printf("  My coordinates minx: %d, miny %d, maxx: %d, maxy: %d\n",
			   tptr->getBBox()->min[0],
			   tptr->getBBox()->min[1],
			   tptr->getBBox()->max[0],
			   tptr->getBBox()->max[1]
			   );
		tptr = tptr->getParent();
	}
}

char *combo_box_entries [] = {"First Entry", "Second Entry", "Third Entry", NULL } ;

char *list_box_entries [] = {"First Entry", "Second Entry", "Third Entry", "Fourth Entry",
	"Sixth Entry", "Seventh Entry", "Eighth Entry", "Ninth Entry", "Tenth Entry", NULL } ;

/* Menu bar entries: */

static char      *file_submenu    [] = {  "Exit", "Close", "========", "Print", "========", "Save", "New", NULL } ;
static puCallback file_submenu_cb [] = { NULL, NULL,       NULL, NULL  ,       NULL,  NULL, NULL, NULL } ;

static char      *edit_submenu    [] = { "Do nothing.", NULL } ;
puCallback edit_submenu_cb [] = {     NULL, NULL } ;

static char      *help_submenu    [] = { "About...",  "Help", NULL } ;
puCallback help_submenu_cb [] = {   NULL, NULL, NULL } ;

char *blist_box_entries [] = { "First Entry", "Second Entry", "Third Entry", "Very long Entry that will get truncated", NULL } ;

char *button_box_entries [] = { "First Entry", "Second Entry", "Third Entry", NULL } ;

ofxpuPopupMenu * popup_menu;

void popup_popup_menu ( puObject * ) {
	popup_menu->reveal();
}

void button_cb ( puObject * ) {
	fprintf ( stderr, "Hello World.\n" ) ;
}

void dial_cb ( puObject * ob ) {
	float my_value;
	ob->getValue(&my_value);
	printf("value %7.2f\n", my_value);
}

void menu_bar_cb (puObject * ob) {
}

//--------------------------------------------------------------
void testApp::setup(){
	ofxpuInit () ;
	ofxpuSetDefaultStyle(PUSTYLE_BOXED);
	
	
	// Column 1
	
	ofxpuMenuBar * menu_bar = new ofxpuMenuBar () ;
	{
		menu_bar -> add_submenu ( "File", file_submenu, file_submenu_cb ) ;
		menu_bar -> add_submenu ( "Edit", edit_submenu, edit_submenu_cb ) ;
		menu_bar -> add_submenu ( "Help", help_submenu, help_submenu_cb ) ;
	}
	menu_bar -> close () ;
	
	ofxpuaVerticalMenu * vertical_menu = new ofxpuaVerticalMenu (0, 130) ;
	{
		vertical_menu -> add_submenu ( "File", file_submenu, file_submenu_cb ) ;
		vertical_menu -> add_submenu ( "Edit", edit_submenu, edit_submenu_cb ) ;
		vertical_menu -> add_submenu ( "Help", help_submenu, help_submenu_cb ) ;
	}
	vertical_menu -> close () ;
	
	ofxpuOneShot *one_shot = new ofxpuOneShot ( 10, 150, 100, 20 );
	one_shot->setLegend   ( "Say Hello" ) ;
	one_shot->setCallback ( button_cb );
	one_shot->setLabel("One Shot");
	one_shot->setLabelPlace(PUPLACE_BOTTOM_CENTER) ;
	
	
	ofxpuFrame *frame = new ofxpuFrame(10.0, 200.0, 100.0, 100.0);
	frame->setLabel("Frame");
	frame->setLabelPlace(PUPLACE_BOTTOM_CENTER) ;
	frame->setLegend("My Legend");
	frame->setLegendPlace(OFXPUPLACE_TOP_CENTERED);
	
	
	ofxpuInput *input = new ofxpuInput(50.0, 340, 100, 30);
	input->setLabelPlace(OFXPUPLACE_CENTERED_LEFT);
	input->setLabel ( "Input" );
	
	ofxpuButton *button1 = new ofxpuButton(10.0, 400.0, "Button 1");
	button1->setCallback(button_cb);
	
	ofxpuButton *button2 = new ofxpuButton(10.0, 450.0, 70.0, 25.0);
	button2->setLegend("Button 2");
	button2->setCallback(button_cb);
	
	ofxpuArrowButton *arrow_button = new ofxpuArrowButton(60.0, 500.0, 25.0, 25.0, OFXPUARROW_DOWN);
	arrow_button->setLabel("Arrow Button");
	arrow_button->setLabelPlace(OFXPUPLACE_BOTTOM_CENTERED);
	arrow_button->setCallback(button_cb);
	
	// Fonts
	
	ofxpuText * textout1 = new ofxpuText(10, 600);
	textout1->setLabel("8 By 13 Font");
	textout1->setLabelFont(PUFONT_8_BY_13);
	
	ofxpuText * textout2 = new ofxpuText(10, 618);
	textout2->setLabel("9 By 15 Font");
	textout2->setLabelFont(PUFONT_9_BY_15);
	
	ofxpuText * textout3 = new ofxpuText(10, 630);
	textout3->setLabel("Helvetica 10");
	textout3->setLabelFont(PUFONT_HELVETICA_10);
	
	ofxpuText * textout4 = new ofxpuText(10, 645);
	textout4->setLabel("Helvetica 12");
	textout4->setLabelFont(PUFONT_HELVETICA_12);
	
	ofxpuText * textout5 = new ofxpuText(10, 665);
	textout5->setLabel("Helvetica 18");
	textout5->setLabelFont(PUFONT_HELVETICA_18);
	
	ofxpuText * textout6 = new ofxpuText(10, 677);
	textout6->setLabel("Times Roman 10");
	textout6->setLabelFont(PUFONT_TIMES_ROMAN_10);
	
	ofxpuText * textout7 = new ofxpuText(10, 705);
	textout7->setLabel("Times Roman 24");
	textout7->setLabelFont(PUFONT_TIMES_ROMAN_24);
	
	// Column 2
	
	// Builds up
	popup_menu = new ofxpuPopupMenu ( 250.0, 115.0 ) ;
	popup_menu->add_item ( "Cut", NULL ) ;
	popup_menu->add_item ( "Copy", NULL ) ;
	popup_menu->add_item ( "Paste", NULL ) ;
	popup_menu->add_item ( "Delete", NULL ) ;
	popup_menu->close () ;
	
	ofxpuButton * button3 = new ofxpuButton(250.0, 120.0, 100.0, 25.0);
	button3->setLegend("Popup Menu");
	button3->setCallback(popup_popup_menu);
	
	ofxpuDial *dial = new ofxpuDial(250.0, 165.0, 50.0, 1.0, 10.0);
	dial->setCallback(dial_cb);
	dial->setLabel("Dial");
	dial->setLabelPlace(PUPLACE_BOTTOM_CENTER);
	
	ofxpuaList *list = new ofxpuaList(250.0, 245.0, 100.0, 70.0, blist_box_entries);
	list->setLabel("List");
	list->setLabelPlace(PUPLACE_BOTTOM_CENTER);
	
	ofxpuButtonBox * button_box = new ofxpuButtonBox ( 250.0, 345.0, 130.0, 70.0, button_box_entries, TRUE ) ;
	button_box->setLabel("Button Box");
	button_box->setLabelPlace(PUPLACE_BOTTOM_CENTER);
	
	
	// Column 3
	
	ofxpuSlider * slider = new ofxpuSlider ( 500, 30, 120, FALSE ) ;
	slider->setLabel ( "Slider" ) ;
	slider->setLabelPlace(PUPLACE_BOTTOM_CENTER) ;
	slider->setLegend ( "Legend" ) ;
	slider->setMaxValue ( 20 ) ;
	slider->setValue ( 4 ) ;
	
	ofxpuaBiSlider * bislider = new ofxpuaBiSlider ( 500, 85, 120, FALSE ) ;
	bislider->setLabel ( "Bi-Slider" ) ;
	bislider->setLabelPlace(PUPLACE_BOTTOM_CENTER) ;
	bislider->setLegend ( "Legend" ) ;
	bislider->setMaxValue ( 20 ) ;
	bislider->setCurrentMin ( 4 ) ;
	bislider->setCurrentMax ( 15 ) ;
	
	ofxpuaTriSlider * trislider = new ofxpuaTriSlider ( 500, 145, 180, FALSE ) ;
	trislider->setLabel ( "TriSlider" ) ;
	trislider->setLabelPlace(PUPLACE_BOTTOM_CENTER) ;
	trislider->setLegend ( "Legend" ) ;
	trislider->setFreezeEnds(FALSE);
	trislider->setMaxValue ( 20 ) ;
	trislider->setCurrentMin ( 4 ) ;
	trislider->setCurrentMax ( 15 ) ;
	trislider->setValue ( 12 ) ;
	
	ofxpuaSliderWithInput * sliderwi = new ofxpuaSliderWithInput ( 550, 180, 40, 100, FALSE ) ;
	sliderwi->setLabel ( "Slider w/ Input" ) ;
	sliderwi->setLabelPlace (PUPLACE_BOTTOM_CENTERED) ;
	sliderwi->setLegend ( "Legend" ) ;
	sliderwi->setMaxValue ( 20 ) ;
	sliderwi->setValue ( 12 ) ;
	sliderwi->setStepSize(1);
	
	ofxpuaBiSliderWithEnds * bisliderwe = new ofxpuaBiSliderWithEnds ( 550, 315, 40, 100 ) ;
	bisliderwe->setLabel ( "BiSlider w/ Ends" ) ;
	bisliderwe->setLabelPlace (PUPLACE_BOTTOM_CENTERED) ;
	bisliderwe->setLegend ( "Legend" ) ;
	bisliderwe->setMaxValue ( 20 ) ;
	bisliderwe->setCurrentMin ( 4 ) ;
	bisliderwe->setCurrentMax ( 15 ) ;
	
	ofxpuaScrollBar * ScrollBar = new ofxpuaScrollBar ( 500, 465, 180, 1, FALSE ) ;
	ScrollBar->setLabel("Scroll Bar");
	ScrollBar->setLabelPlace(OFXPUPLACE_BOTTOM_CENTERED);
	ScrollBar->setMaxValue ( 20 ) ;
	ScrollBar->setValue ( 12 ) ;
	
	ofxpuaSpinBox * spinbox1 = new ofxpuaSpinBox (550, 525, 60, 30);
	spinbox1->setLabel("Spin Box");
	spinbox1->setLabelPlace(OFXPUPLACE_BOTTOM_CENTERED);
	
	ofxpuaSelectBox * select_box = new ofxpuaSelectBox(500, 585, 200, 30, list_box_entries);
	select_box->setLabelPlace(OFXPUPLACE_BOTTOM_CENTERED);
	select_box->setLabel("Select Box");
	
	// Column 4
	
	// Resizing messes up test wrap.
	ofxpuaLargeInput * large_input = new ofxpuaLargeInput ( 730, 30, 240, 180, 2, 20, FALSE ) ;
	large_input->setLabelPlace(OFXPUPLACE_BOTTOM_CENTERED);
	large_input->setLabel("Large Input Box");
	large_input->setText("This is text in the Large Input widget.\n"
                         "This is a second line of text");
	//large_input->ofxsetPosition(600, 10);
	//large_input->ofxsetSize(100, 100);
	
	ofxpuaList * scrolling_list = new ofxpuaList(730, 260, 200, 100, list_box_entries, 20);
	scrolling_list->setLabelPlace(OFXPUPLACE_BOTTOM_CENTERED);
	scrolling_list->setLabel("Scrolling List");
	
	ofxpuaComboBox * combo_box = new ofxpuaComboBox(730, 410, 250, 32, combo_box_entries) ;
	/* Don't do this in your own application ! */
	combo_box -> __getPopupMenu()->reveal() ;
	combo_box->setLabelPlace(OFXPUPLACE_BOTTOM_RIGHT);
	combo_box->setLabel ("Combo Box");
	
	// Takes focus and nothing else works until it is removed
	//ofxpuaFileSelector * file_selector = new ofxpuaFileSelector (730, 545, 280, 200, ".");
	//file_selector->setLabelPlace(OFXPUPLACE_BOTTOM_CENTERED);
	//file_selector->setLabel("File Selector");
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	
	EXACT_PIXEL_BEGIN
	
	ofNoFill();
	ofSetHexColor(0xff0000);
	
	EXACT_PIXEL_END
	
	ofxpuDisplay ();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	ofxpuKeyboard ( key, PU_DOWN ) ;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	ofxpuMouse ( x, y );
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	ofxpuMouse ( x, y );
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	ofxpuMouse ( button, GLUT_DOWN, x, y );
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	ofxpuMouse ( button, GLUT_UP, x, y );
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

