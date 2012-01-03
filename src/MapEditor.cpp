#ifndef MAPEDITOR_CPP
#define MAPEDITOR_CPP
#include "MapEditor.h"
#include "Config.h"
#include "Camera.h"
#include "InputControls.h"
#include "BackgroundManager.h"
extern SDL_Surface *screen;
extern SDL_Event event;
extern C_Camera Camera;
extern C_ImageList LevelAssets;
extern C_ImageList BackgroundImg;
extern C_InputControls Controls;
extern C_BackgroundManager BackgroundManager;

extern bool MapEditorActive;
extern bool MapEditorMenuActive;
extern bool mapMode;
extern float tempMouseX;
extern float tempMouseY;


C_MapEditor::C_MapEditor(C_TileManager* tileManagerPointer, std::vector<C_BaseEntity*>* entityPointer, C_GUI* guiPointer)
{
	
	m_menuActive = false;

	//Pointers
	p_tileManager = tileManagerPointer;			//TileManager pointer
	pv_Entities = entityPointer;				//Pointer to entity vector
	p_GUI = guiPointer;							//GUI pointer
	

	//Mouse variables INIT
	SDL_ShowCursor(SDL_DISABLE);
	m_mouseX = 0;
	m_mouseY = 0;
	highlightStart.x = 0;
	highlightStart.y = 0;
	highlightStop.x = 0;
	highlightStop.y = 0;
	highlightArea.x = 0;
	highlightArea.y = 0;
	highlightArea.w = 0;
	highlightArea.h = 0;
	m_highlight = false;
	m_showHighlight = false;
	m_showPaste = false;

	//Menu Stuff
	m_menuX = 0;
	m_menuY = 0;

	lock_row1 = true;
	lock_row2 = true;
	lock_row3 = true;
	lock_row4 = true;
	lock_row5 = true;
	lock_row6 = true;
	lock_row7 = true;
	lock_row8 = true;
	lock_row9 = true;

	//Tile variables
	m_tileType = 0;
	m_plane = 1;
	m_destructable = false;
	m_solid = true;
	m_friendlyDmg = false;
	m_enemyDmg = false;
	m_colorR = 255;
	m_colorG = 255;
	m_colorB = 255;
	m_colorA = 255;
	m_tileColor = SDL_MapRGBA(screen->format,  m_colorR, m_colorG, m_colorB, m_colorA);

	m_tileBuildSize = 2;


	m_currentMenu = MENU_MAIN;
	m_rowSelected = 1;//row selected in the menu
	m_numOfRows = 1;
	
	//Warp menu variables
	warpSelected = 0;
	//BG menu variables
	m_bgSelected = 0;
	m_bgTypeShowing = 0;

	//Camera tracking
	m_trackPlayer = true;
	m_trackToX = 0;
	m_trackToY = 0;

	//DISPLAY AND HOLD IMAGES ETC
	TileSprites.LoadImage("Tiles\\Tiles.png");

	//MAP STUFF
	m_mapAlphaCounter = 0;
}

C_MapEditor::~C_MapEditor()
{

}
void C_MapEditor::Start()
{
	MapEditorActive = true;

	//if on display some txt to screen showing that it's on
	std::stringstream output;
	output << "MAP EDITOR RUNNIN";
	Font.Print(&output, screen, 12, 0, 0);
	
	
	Update();//MEAT & POTATOES
}
void C_MapEditor::Stop()
{
	MapEditorActive = false;
}

void C_MapEditor::Update()//General inputs 
{
	//Key commands
	if(Controls.escape)
		m_currentMenu = MENU_MAIN;
	


	HandleMouse();//check inputs & actions

	HighlightOptions();//handle options while doing highlights

	//Map stuff
	if(mapMode)
		WorldMapNav(); //allow instant movement around world map

	if(m_menuActive)
		switch(m_currentMenu){
			case MENU_MAIN: MainMenu();
							break;
			case TILE_MAIN: TilesMenu();
							break;
					case TILE_CHANGE: Tiles_EditType();//change new tile types
									break;
					case TILE_PLANE: Tiles_EditPlane();//edit new tile plane
									break;
					case TILE_DESTRUCTABLE: Tiles_EditDestructable();//edit new tile plane
									break;
					case TILE_SOLID: Tiles_EditSolid();//edit new tile plane
									break;
					case TILE_FRIENDLYDAMAGE: Tiles_EditFriendlyDmg();//edit new tile plane
									break;
					case TILE_ENEMYDAMAGE: Tiles_EditEnemyDmg();//edit new tile plane
									break;
					case TILE_BUILD_TILE_SET: Tiles_EditTileSet();//edit/create new tile sets
									break;
			case WARPS_MAIN: WarpMenu();//edit objects
									break;
			case BG_OBJECTS_MAIN: BG_ObjectsMenu();//Background object menu
									break;
					

			default:		MainMenu();
							break;
		}
}

void C_MapEditor::HandleMouse()
{
	//Cursor
	SDL_Rect cursorPosition= {(m_mouseX - Camera.Get_XYWH().x), (m_mouseY - Camera.Get_XYWH().y)};

	SDL_Rect cursorIcon = {m_tileType * 16, m_tileType/12 * 16, 16, 16};
	if(cursorIcon.y > 0)
		cursorIcon.x -= cursorIcon.y * 12;
	if(m_tileType == -1)
		cursorIcon.x = 0;
	SDL_BlitSurface(LevelAssets.getImage(0), &cursorIcon, screen, &cursorPosition);

	//Highlights ****fabulious?!
	if(!Controls.rightClick && m_highlight == true){//if not right clicking update where a highlight can start
		m_highlight = false;
	}
	if(Controls.rightClick){			//when pressing right click
		if(m_highlight == false){		//if not already highlighting
			highlightStart.x = int(m_mouseX / TILE_SIZE) * TILE_SIZE;//update start position
			highlightStart.y = int(m_mouseY / TILE_SIZE) * TILE_SIZE;
			m_highlight = true;			//start highlighting
			m_showHighlight = true;
		}
		highlightStop.x = int(m_mouseX / TILE_SIZE) * TILE_SIZE;
		highlightStop.y = int(m_mouseY / TILE_SIZE) * TILE_SIZE;

		highlightArea.x = highlightStart.x - Camera.Get_XYWH().x;
		highlightArea.y = highlightStart.y - Camera.Get_XYWH().y;
		highlightArea.w = highlightStop.x - Camera.Get_XYWH().x;
		highlightArea.h = highlightStop.y - Camera.Get_XYWH().y;
	}

	//SELECT ALL
	if(Controls.key_lctrl ){//&& (Controls.key_lctrl || Controls.key_rctrl)){
		highlightArea.x = 0;
		highlightArea.y = 0;
		highlightArea.w = SCREEN_WIDTH;
		highlightArea.h = SCREEN_HEIGHT;

		m_showHighlight = true;
	}

	//Draw highlights!!!!
	if(m_showHighlight){//if highlighting
		boxRGBA(screen, highlightArea.x , highlightArea.y, highlightArea.w, highlightArea.h, 0, 0, 200, 100);//draw highlight box
	}

	//********* End Highlights******

	//Poll mouse
	if( event.type == SDL_MOUSEMOTION ){ 
		m_mouseX = event.motion.x + Camera.Get_XYWH().x;//update mouse x,y position
        m_mouseY = event.motion.y + Camera.Get_XYWH().y;
    }

	//Output mouse info
	std::stringstream mouse_info;
	mouse_info //<< "Mouse data -- X/Y: " << m_mouseX << " / " << m_mouseY;
			   //<< "      |  " << Camera.Get_XYWH().x << "/ " << Camera.Get_XYWH().y << "  TileType: " << m_tileType 
			   //<< "     tile position: " << m_mouseX/16 << " / " << m_mouseY/16 << "   menu pos: " << m_menuX << " / " << m_menuY;
			   //<< "   Row Selected: " << m_rowSelected << "    CurrentMenu: " << m_currentMenu 
			   //<< "  Right Clicking?: " << Controls.rightClick << "  highlightStartX/Y: " << highlightStart.x << " / " << highlightStart.y << "  highlightStopX/Y: " << highlightStop.x << " / " << highlightStop.y ;
			   //<< "  bufferOffset x/y:  " << p_tileManager->Get_tileBufferOffset().x << " / " << p_tileManager->Get_tileBufferOffset().y;
			   //<< "Map Position ( " << p_tileManager->Get_mapPosition().x << " , " << p_tileManager->Get_mapPosition().y << " )   zone type: " << p_tileManager->Get_mapType() << "OBJ TYPE: " << m_bgTypeShowing;
			   << " Map Mode mouse x/y: " <<int((m_mouseX - Camera.Get_XYWH().x)*tempMouseX) %160 << " | " << (m_mouseY - Camera.Get_XYWH().y)*tempMouseY;
	Font.Print(&mouse_info, screen, 12, 0, 16);

	//Handle clicks //TODO seperate function for this
	//Lay tiles
	if(Controls.leftClick && m_menuActive == false){
		p_tileManager->Set_tileType(m_mouseX/TILE_SIZE, m_mouseY/TILE_SIZE, m_tileType);//set tile type to this
		if(m_tileType == -1)
			p_tileManager->Set_tileSolid(m_mouseX/TILE_SIZE, m_mouseY/TILE_SIZE, OFF);
	}


	//OPEN MENU///////////////////////////////////
	if(Controls.middleClick){
		MapEditorMenuActive = !MapEditorMenuActive;
		m_menuActive = !m_menuActive;
		m_menuX = 400;
		m_menuY =  SCREEN_HEIGHT - 16*m_numOfRows;
	}

	//Scroll through tiles / BG objects
	if(Controls.wheelUp && Controls.key_lshift){
		m_tileType++;
		if(m_tileType > MAX_TILE_COUNT)
			m_tileType = -1;
		if(m_currentMenu == BG_OBJECTS_MAIN){
			m_bgTypeShowing++;
		}
	}
	if(Controls.wheelDown && Controls.key_lshift){
		m_tileType--;
		if(m_tileType < -1)
			m_tileType = MAX_TILE_COUNT;
		if(m_currentMenu == BG_OBJECTS_MAIN){
			m_bgTypeShowing--;
		}
	}

	//AUTO REPOSITION MENU
	if(m_mouseX - Camera.Get_XYWH().x > 600)
		m_menuX = 0;
	if(m_mouseX - Camera.Get_XYWH().x < 40)
		m_menuX = 400;
	if(m_mouseY - Camera.Get_XYWH().y > 440)
		m_menuY = 64;
	if(m_mouseY - Camera.Get_XYWH().y < 40)
		m_menuY =  SCREEN_HEIGHT - 16*m_numOfRows;

	//if on bottom reposition when it expands
	if(m_menuY != 64)
		m_menuY =  SCREEN_HEIGHT - 16*m_numOfRows;
}

void C_MapEditor::HighlightOptions()
{
	//DRAWING TILE TO HIGHLIGHTED AREA
	if(m_showHighlight)//if something is highlighted
		if(Controls.key_S){//if pressing S while stuff is highlighted draw stuff
			for(int x = highlightArea.x / TILE_SIZE + Camera.Get_XYWH().x / TILE_SIZE; x < highlightArea.w / TILE_SIZE + Camera.Get_XYWH().x / TILE_SIZE; x++)
				for(int y = highlightArea.y / TILE_SIZE + Camera.Get_XYWH().y / TILE_SIZE; y < highlightArea.h / TILE_SIZE + Camera.Get_XYWH().y / TILE_SIZE; y++)
					p_tileManager->Set_tileType(x, y, m_tileType);//set tile type to this
			m_showHighlight = false;
		}

	//DELETING TILE FROM HIGHLIGHTED AREA
	if(m_showHighlight)//if something is highlighted
		if(Controls.key_delete){//if pressing D while stuff is highlighted delete stuff
			for(int x = highlightArea.x / TILE_SIZE + Camera.Get_XYWH().x / TILE_SIZE; x < highlightArea.w / TILE_SIZE + Camera.Get_XYWH().x / TILE_SIZE; x++)
				for(int y = highlightArea.y / TILE_SIZE + Camera.Get_XYWH().y / TILE_SIZE; y < highlightArea.h / TILE_SIZE + Camera.Get_XYWH().y / TILE_SIZE; y++)
					p_tileManager->Set_tileType(x, y, -1);//set tile type to this
			m_showHighlight = false;
		}

	//COPYING TILES
	C_Tiles currentTile;
	if(m_showHighlight)//if something is highlighted
		if(Controls.key_C)//if pressing C you can copy what is highlighted into the 'v2d_tileBuffer'
			for(int x = 0; x < (highlightArea.w - highlightArea.x)/ TILE_SIZE /*+ Camera.Get_XYWH().x / TILE_SIZE*/; x++){
				v2d_tileBuffer.push_back( std::vector<C_Tiles>() ); //Push a C_Tiles vector
				for(int y = 0; y < (highlightArea.h - highlightArea.y) / TILE_SIZE/* + Camera.Get_XYWH().y / TILE_SIZE*/; y++){
					
					currentTile.Set_tileWidth(TILE_SIZE);
					currentTile.Set_tileHeight(TILE_SIZE);
					currentTile.Set_tileType(p_tileManager->Get_tileType((highlightArea.x + Camera.Get_XYWH().x)/TILE_SIZE + x, (highlightArea.y + Camera.Get_XYWH().y)/TILE_SIZE + y ) ); //adding camera position to get position of tile even when scrolling
					currentTile.Set_destructable(false);
					currentTile.Set_solid(false);
					currentTile.Set_friendlyDamage(false);
					currentTile.Set_enemyDamage(false);

					currentTile.Set_tileX(x * TILE_SIZE);
					currentTile.Set_tileY(y * TILE_SIZE);

					//end temp
					v2d_tileBuffer[x].push_back(currentTile);///LOAD tiles starting top/left and move right
				}
			m_showHighlight = false;
			m_showPaste = true;
			}

	//DISPLAY WHATS PASTING
	SDL_Rect offset;
	SDL_Rect tileClip = {0,0, 16, 16};
	if(m_showPaste == true)//change to something else later TODO
		for(int x = 0; x < v2d_tileBuffer.size(); x++)
			for(int y = 0; y < v2d_tileBuffer[x].size(); y++)
			{
				m_showPaste = true;
				tileClip.x = 16 * v2d_tileBuffer[x][y].Get_tileType() - (v2d_tileBuffer[x][y].Get_tileType()/12 * 16 * 12);//12 is the amount of tiles per row
				tileClip.y = 16 * (v2d_tileBuffer[x][y].Get_tileType()/12); //12 tiles per row in the sprite sheet.
				
				//AND finally draw the tiles
				if(m_showPaste == true){
					offset.x = m_mouseX + v2d_tileBuffer[x][y].Get_tileX() - Camera.Get_XYWH().x;
					offset.y = m_mouseY + v2d_tileBuffer[x][y].Get_tileY() - Camera.Get_XYWH().y;	

					//Getto stop to prevent overflow on vector
					if((m_mouseX + highlightArea.w - highlightArea.x) > LEVEL_WIDTH)
						m_mouseX = LEVEL_WIDTH - (highlightArea.w - highlightArea.x);//prevent from pasting too far right
					if((m_mouseY + highlightArea.h - highlightArea.y) > LEVEL_HEIGHT)
						m_mouseY = LEVEL_HEIGHT - (highlightArea.h - highlightArea.y);//prevent from pasting too far down

					SDL_BlitSurface(LevelAssets.getImage(0), &tileClip, screen, &offset);
				}
			}


			
	//ACTUALLY PASTE NOW
	//SDL_Rect pasteOffset = { offset.x, offset.y};
	if(Controls.key_V)
		for(int x = 0; x < v2d_tileBuffer.size(); x++)
			for(int y = 0; y < v2d_tileBuffer[x].size(); y++){
				
				//Paste these to tiles on screen
				p_tileManager->Set_tileType(m_mouseX / TILE_SIZE + x, m_mouseY / TILE_SIZE + y, v2d_tileBuffer[x][y].Get_tileType() );
			}
	
	//DEBUG~ISH STUFF
	if(Controls.escape){//TODO: Make this auto clear
		v2d_tileBuffer.clear();
	}

	if(m_currentMenu != BG_OBJECTS_MAIN)//Turn highlights off if not in this menu
		for(int i = 0; i < BackgroundManager.ActiveSize(); i++)
			BackgroundManager.Set_highlight(m_bgSelected, OFF);
}

void C_MapEditor::MainMenu()
{
	m_numOfRows = 4;//row count for main menu
	
	NavigateMenus();

	//Display Menu Options
	row1 << "----MAIN MENU----";
	row2 << "TILES -> edit/change/etc";
	row3 << "WARPS -> edit/change/etc";
	row4 << "BACKGROUND OBECTS -> edit/etc";
	
	
	Font.Print(&row1, screen, 12, menuPosition.x, menuPosition.y + 0 );
	Font.Print(&row2, screen, 12, menuPosition.x, menuPosition.y + 16);
	Font.Print(&row3, screen, 12, menuPosition.x, menuPosition.y + 32);
	Font.Print(&row4, screen, 12, menuPosition.x, menuPosition.y + 48);
	//Font.Print(&row5, screen, 12, menuPosition.x, menuPosition.y + 64);

	//What to do when a row is selected
	if(Controls.enter || Controls.leftClick)
		switch(m_rowSelected){
			case 2: m_currentMenu = TILE_MAIN;
					break;
			case 3: m_currentMenu = WARPS_MAIN;
					break;
			case 4: m_currentMenu = BG_OBJECTS_MAIN;
					break;
		}
}

void C_MapEditor::TilesMenu()
{
	m_numOfRows = 9;//row count for main menu
	
	NavigateMenus();

	//Display Menu Options
	row1 << "TILES MENU - edit values for new tiles";//just a header
	row2 << "EDIT TILE TYPE";
	row3 << "EDIT TILE PLANE (height)";
	row4 << "EDIT DESTRUCTABLE";
	row5 << "EDIT SOLID";
	row6 << "EDIT FRIENDLY DAMAGE";
	row7 << "EDIT ENEMY DAMAGE";
	row8 << "**EDIT TILE SET**";
	row9 << "GO BACK";

	Font.Print(&row1, screen, 12, menuPosition.x, menuPosition.y + 0 );
	Font.Print(&row2, screen, 12, menuPosition.x, menuPosition.y + 16);
	Font.Print(&row3, screen, 12, menuPosition.x, menuPosition.y + 32);
	Font.Print(&row4, screen, 12, menuPosition.x, menuPosition.y + 48);
	Font.Print(&row5, screen, 12, menuPosition.x, menuPosition.y + 64);
	Font.Print(&row6, screen, 12, menuPosition.x, menuPosition.y + 80);
	Font.Print(&row7, screen, 12, menuPosition.x, menuPosition.y + 96);
	Font.Print(&row8, screen, 12, menuPosition.x, menuPosition.y + 112);
	Font.Print(&row9, screen, 12, menuPosition.x, menuPosition.y + 128);

	if(Controls.enter || Controls.leftClick)
		switch(m_rowSelected){
			case 1: m_currentMenu = TILE_MAIN;
					break;
			case 2: m_currentMenu = TILE_CHANGE;
					break;
			case 3: m_currentMenu = TILE_PLANE;
					break;
			case 4: m_currentMenu = TILE_DESTRUCTABLE;
					break;
			case 5: m_currentMenu = TILE_SOLID;
					break;
			case 6: m_currentMenu = TILE_FRIENDLYDAMAGE;
					break;
			case 7: m_currentMenu = TILE_ENEMYDAMAGE;
					break;
			case 8: m_currentMenu = TILE_BUILD_TILE_SET;				
					break;
			case 9: m_currentMenu = MENU_MAIN;				
					break;
		}

}
void C_MapEditor::Tiles_EditType()
{
	//do something cool to show tiles available
	m_numOfRows = 3;//row count for main menu
	
	NavigateMenus();

	//SHOW OPTIONS
	row1 << "--CHANGE TYPE OF TILE TO PLOT---";
	row2 << "NOT DONE YET";
	row3 << "GO BACK";

	Font.Print(&row1, screen, 12, menuPosition.x, menuPosition.y + 0 );
	Font.Print(&row2, screen, 12, menuPosition.x, menuPosition.y + 16);
	Font.Print(&row3, screen, 12, menuPosition.x, menuPosition.y + 32);

	if(Controls.enter || Controls.leftClick)
		switch(m_rowSelected){
			case 3: m_currentMenu = TILE_MAIN;
					break;
		}


}
void C_MapEditor::Tiles_EditPlane()
{
	m_numOfRows = 7;//row count for main menu
	
	NavigateMenus();

	//SHOW OPTIONS
	row1 << "--EDIT PLANE VALUE. CURRENTLY: " << m_plane;	//just a header
	row2 << "CHANGE TO 1";
	row3 << "CHANGE TO 2";
	row4 << "CHANGE TO 3";
	row5 << "CHANGE TO 4";
	row6 << "CHANGE TO 5";
	row7 << "GO BACK";

	Font.Print(&row1, screen, 12, menuPosition.x, menuPosition.y + 0 );
	Font.Print(&row2, screen, 12, menuPosition.x, menuPosition.y + 16);
	Font.Print(&row3, screen, 12, menuPosition.x, menuPosition.y + 32);
	Font.Print(&row4, screen, 12, menuPosition.x, menuPosition.y + 48);
	Font.Print(&row5, screen, 12, menuPosition.x, menuPosition.y + 64);
	Font.Print(&row6, screen, 12, menuPosition.x, menuPosition.y + 80);
	Font.Print(&row7, screen, 12, menuPosition.x, menuPosition.y + 96);
	
	if(Controls.enter || Controls.leftClick)
		switch(m_rowSelected){
			case 2: m_plane = 1;
					break;
			case 3: m_plane = 2;
					break;
			case 4: m_plane = 3;
					break;
			case 5: m_plane = 4;
					break;
			case 6: m_plane = 5;
					break;
			case 7: m_currentMenu = TILE_MAIN;
					break;
		}
	
	
}
void C_MapEditor::Tiles_EditDestructable()
{
	//do something cool to show tiles available
	m_numOfRows = 3;//row count for main menu
	
	NavigateMenus();
	std::string destructValue = "NO";
	if(m_destructable)
		destructValue = "YES";
	//SHOW OPTIONS
	row1 << "---DESTRUCTABLE TILES?---" << destructValue << "---";
	row2 << "Toggle if destructable";
	row3 << "GO BACK";

	Font.Print(&row1, screen, 12, menuPosition.x, menuPosition.y + 0 );
	Font.Print(&row2, screen, 12, menuPosition.x, menuPosition.y + 16);
	Font.Print(&row3, screen, 12, menuPosition.x, menuPosition.y + 32);

	if(Controls.enter || Controls.leftClick)
		switch(m_rowSelected){
			case 2: m_destructable = !m_destructable;
					break;
			case 3: m_currentMenu = TILE_MAIN;
					break;
		}


}
void C_MapEditor::Tiles_EditSolid()
{
	//do something cool to show tiles available
	m_numOfRows = 3;//row count for main menu
	
	NavigateMenus();
	std::string solidValue = "YES";
	if(!m_solid)
		solidValue = "NO";
	//SHOW OPTIONS
	row1 << "---SOLID TILES?---" << solidValue << "---";
	row2 << "Toggle if solid";
	row3 << "GO BACK";

	Font.Print(&row1, screen, 12, menuPosition.x, menuPosition.y + 0 );
	Font.Print(&row2, screen, 12, menuPosition.x, menuPosition.y + 16);
	Font.Print(&row3, screen, 12, menuPosition.x, menuPosition.y + 32);

	if(Controls.enter || Controls.leftClick)
		switch(m_rowSelected){
			case 2: m_solid = !m_solid;
					break;
			case 3: m_currentMenu = TILE_MAIN;
					break;
		}

}
void C_MapEditor::Tiles_EditFriendlyDmg()
{
	//do something cool to show tiles available
	m_numOfRows = 3;//row count for main menu
	
	NavigateMenus();
	std::string friendlyDmgValue = "NO";
	if(!m_friendlyDmg)
		friendlyDmgValue = "YES";
	//SHOW OPTIONS
	row1 << "---FRIENDLY DAMAGE TILES?---" << friendlyDmgValue << "---";
	row2 << "TOGGLE IF TILES DOES FRIENDLY DAMAGE";
	row3 << "GO BACK";

	Font.Print(&row1, screen, 12, menuPosition.x, menuPosition.y + 0 );
	Font.Print(&row2, screen, 12, menuPosition.x, menuPosition.y + 16);
	Font.Print(&row3, screen, 12, menuPosition.x, menuPosition.y + 32);

	if(Controls.enter || Controls.leftClick)
		switch(m_rowSelected){
			case 2: m_friendlyDmg = !m_friendlyDmg;
					break;
			case 3: m_currentMenu = TILE_MAIN;
					break;
		}
}
void C_MapEditor::Tiles_EditEnemyDmg()
{
	//do something cool to show tiles available
	m_numOfRows = 3;//row count for main menu
	
	NavigateMenus();
	std::string enemyDmgValue = "NO";
	if(!m_enemyDmg)
		enemyDmgValue = "YES";
	//SHOW OPTIONS
	row1 << "---ENEMY DAMAGE TILES?---" << enemyDmgValue << "---";
	row2 << "TOGGLE IF TILES DOES ENEMY DAMAGE";
	row3 << "GO BACK";

	Font.Print(&row1, screen, 12, menuPosition.x, menuPosition.y + 0 );
	Font.Print(&row2, screen, 12, menuPosition.x, menuPosition.y + 16);
	Font.Print(&row3, screen, 12, menuPosition.x, menuPosition.y + 32);

	if(Controls.enter || Controls.leftClick)
		switch(m_rowSelected){
			case 2: m_enemyDmg = !m_enemyDmg;
					break;
			case 3: m_currentMenu = TILE_MAIN;
					break;
		}
}
void C_MapEditor::Tiles_EditTileSet()
{
		//do something cool to show tiles available
	m_numOfRows = 9;//row count for main menu
	
	NavigateMenus();
	
	//SHOW OPTIONS
	row1 << "---EDIT TILE SET---";
	row2 << "2 x 2";
	row3 << "3 x 3";
	row4 << "4 x 4";
	row5 << "5 x 5";
	row6 << "6 x 6";
	row7 << "7 x 7";
	row8 << "8 x 8";
	row9 << "----GO BACK----";

	Font.Print(&row1, screen, 12, menuPosition.x, menuPosition.y + 0 );
	Font.Print(&row2, screen, 12, menuPosition.x, menuPosition.y + 16);
	Font.Print(&row3, screen, 12, menuPosition.x, menuPosition.y + 32);
	Font.Print(&row4, screen, 12, menuPosition.x, menuPosition.y + 48);
	Font.Print(&row5, screen, 12, menuPosition.x, menuPosition.y + 64);
	Font.Print(&row6, screen, 12, menuPosition.x, menuPosition.y + 80);
	Font.Print(&row7, screen, 12, menuPosition.x, menuPosition.y + 96);
	Font.Print(&row8, screen, 12, menuPosition.x, menuPosition.y + 112);
	Font.Print(&row9, screen, 12, menuPosition.x, menuPosition.y + 128);

	if(Controls.enter || Controls.leftClick)
		switch(m_rowSelected){
			case 2: m_tileBuildSize = 2;
					break;
			case 3: m_tileBuildSize = 3;
					break;
			case 4: m_tileBuildSize = 4;
					break;
			case 5: m_tileBuildSize = 5;
					break;
			case 6: m_tileBuildSize = 6;
					break;
			case 7: m_tileBuildSize = 7;
					break;
			case 8: m_tileBuildSize = 8;
					break;
			case 9: m_currentMenu = TILE_MAIN;
					break;
		}

	//Draw tile grid used to make a new set
	SDL_Rect buildGrid = {m_menuX, m_menuY, TILE_SIZE * m_tileBuildSize, TILE_SIZE * m_tileBuildSize + 16};
	boxRGBA(screen, menuPosition.x, menuPosition.y - buildGrid.h, menuPosition.x + buildGrid.w, menuPosition.y - 16, 0, 0, 0, 200);

	//Show tiles that can be selected TODO:(seperate into a function)
	SDL_Rect tileGrid = {0, m_menuY, m_menuX, m_menuY + 9 * TILE_SIZE};
	boxRGBA(screen, tileGrid.x, tileGrid.y, tileGrid.w, tileGrid.h, 0, 0, 0, 240);

	//HARD CODED FOR NOW////////////////////////////////
	
	

	for(int i = 0; i < 4; i++){
		SDL_Rect green ={ 16*i, 0, 16, 16};
		SDL_Rect position = { 16 + 32 *i, m_menuY + 16 };
		SDL_BlitSurface(TileSprites.getImage(0), &green, screen, &position);
			

	}
	//////////////////////////////////////////////////////


}

void C_MapEditor::WarpMenu()
{
	m_numOfRows = 9;//row count for main menu

	//?????? even needed
	if(warpSelected >= p_tileManager->Get_WarpCount())
		warpSelected = 0; //don't let imaginary things be selected

	//start by highlighting the current warp
	if(p_tileManager->Get_WarpCount() > 0)
		p_tileManager->ToggleWarpHighlight(warpSelected, ON);//start highlight of new

	
	std::string warpType = "";
	std::string rowLock4 = "       --LOCKED--";
	std::string rowLock5 = "       --LOCKED--";
	std::string rowLock6 = "       --LOCKED--";
	

	if(p_tileManager->Get_WarpType(warpSelected) == OVERWORLD)
		warpType = "Over world";
	if(p_tileManager->Get_WarpType(warpSelected) == DUNGEON)
		warpType = "Dungeon";
	if(p_tileManager->Get_WarpType(warpSelected) == CAVE)
		warpType = "Cave";

	//LOCKS
	if(!lock_row4)
		rowLock4 = "       -UNLOCKED-";
	if(!lock_row5)
		rowLock5 = "       -UNLOCKED-";
	if(!lock_row6)
		rowLock6 = "       -UNLOCKED-";

	
	NavigateMenus();

	//Display Menu Options
	row1 << "WARP MENU - edit values for new tiles";//just a header
	row2 << "ADD WARP";
	if(p_tileManager->Get_WarpCount() > 0){
		row3 << "Toggle Warp type: " << warpType;
		row4 << "Warp Position X|Y: " << p_tileManager->Get_WarpPosition(warpSelected).x << " | " << p_tileManager->Get_WarpPosition(warpSelected).y << rowLock4;
		row5 << "Map destination X|Y: " << p_tileManager->Get_WarpDestination(warpSelected).x << " | " << p_tileManager->Get_WarpDestination(warpSelected).y << rowLock5;
		row6 << "End Map location: " << p_tileManager->Get_WarpEndMap(warpSelected).x << " - " << p_tileManager->Get_WarpEndMap(warpSelected).y << rowLock6;
	}
	row7 << "Number of warps on this map: " << p_tileManager->Get_WarpCount();
	row8 << "Cycle through warps - Current: " << warpSelected;
	row9 << "---------DELETE SELECTED WARP----------";
	
	Font.Print(&row1, screen, 12, menuPosition.x, menuPosition.y + 0 );
	Font.Print(&row2, screen, 12, menuPosition.x, menuPosition.y + 16);
	Font.Print(&row3, screen, 12, menuPosition.x, menuPosition.y + 32);
	Font.Print(&row4, screen, 12, menuPosition.x, menuPosition.y + 48);
	Font.Print(&row5, screen, 12, menuPosition.x, menuPosition.y + 64);
	Font.Print(&row6, screen, 12, menuPosition.x, menuPosition.y + 80);
	Font.Print(&row7, screen, 12, menuPosition.x, menuPosition.y + 96);
	Font.Print(&row8, screen, 12, menuPosition.x, menuPosition.y + 112);
	Font.Print(&row9, screen, 12, menuPosition.x, menuPosition.y + 128);


	//EDITING VALUES IN MENU
	SDL_Rect updateLoc = {0,0,0,0};
	//Edit Warp Position
	if(!lock_row4){
		updateLoc.x = m_mouseX;
		updateLoc.y = m_mouseY;
		p_tileManager->Set_WarpPosition(warpSelected, updateLoc);
	}

	//Edit Object End destination and map destination

	if(Controls.up && (!lock_row5 || !lock_row6)){
		if(!lock_row5){
			updateLoc.x = p_tileManager->Get_WarpDestination(warpSelected).x;
			updateLoc.y = p_tileManager->Get_WarpDestination(warpSelected).y - 1;
			p_tileManager->Set_WarpEndPosition(warpSelected, updateLoc);
		}
		if(!lock_row6){
			updateLoc.x = p_tileManager->Get_WarpEndMap(warpSelected).x;
			updateLoc.y = p_tileManager->Get_WarpEndMap(warpSelected).y - 1;
			p_tileManager->Set_WarpEndMap(warpSelected, updateLoc);
		}
	}
	if(Controls.down && (!lock_row5 || !lock_row6)){
		if(!lock_row5){
			updateLoc.x = p_tileManager->Get_WarpDestination(warpSelected).x;
			updateLoc.y = p_tileManager->Get_WarpDestination(warpSelected).y + 1;
			p_tileManager->Set_WarpEndPosition(warpSelected, updateLoc);
		}
		if(!lock_row6){
			updateLoc.x = p_tileManager->Get_WarpEndMap(warpSelected).x;
			updateLoc.y = p_tileManager->Get_WarpEndMap(warpSelected).y + 1;
			p_tileManager->Set_WarpEndMap(warpSelected, updateLoc);
		}
	}
	if(Controls.left && (!lock_row5 || !lock_row6)){
		if(!lock_row5){
			updateLoc.x = p_tileManager->Get_WarpDestination(warpSelected).x - 1;
			updateLoc.y = p_tileManager->Get_WarpDestination(warpSelected).y;
			p_tileManager->Set_WarpEndPosition(warpSelected, updateLoc);
		}
		if(!lock_row6){
			updateLoc.x = p_tileManager->Get_WarpEndMap(warpSelected).x - 1;
			updateLoc.y = p_tileManager->Get_WarpEndMap(warpSelected).y;
			p_tileManager->Set_WarpEndMap(warpSelected, updateLoc);
		}
	}
	if(Controls.right && (!lock_row5 || !lock_row6)){		
		if(!lock_row5){
			updateLoc.x = p_tileManager->Get_WarpDestination(warpSelected).x + 1;
			updateLoc.y = p_tileManager->Get_WarpDestination(warpSelected).y;
			p_tileManager->Set_WarpEndPosition(warpSelected, updateLoc);
		}
		if(!lock_row6){
			updateLoc.x = p_tileManager->Get_WarpEndMap(warpSelected).x + 1;
			updateLoc.y = p_tileManager->Get_WarpEndMap(warpSelected).y;
			p_tileManager->Set_WarpEndMap(warpSelected, updateLoc);
		}
		
	}
	if(Controls.enter || Controls.leftClick)
		switch(m_rowSelected){
		case 2: p_tileManager->Create_Warp();
					break;
		case 3: p_tileManager->Set_WarpType(warpSelected, p_tileManager->Get_WarpType(warpSelected)+1);//toggle through warp types
					break;
		case 4: lock_row4 = !lock_row4;//toggle lock for row 4
					break;
		case 5: lock_row5 = !lock_row5;//toggle lock for row 5
					break;
		case 6: lock_row6 = !lock_row6;//toggle lock for row 6 variables
					break;
		case 8: p_tileManager->ToggleWarpHighlight(warpSelected, OFF);//stop highlight of current
				warpSelected++;//cycle through available warps.
				if(warpSelected >= p_tileManager->Get_WarpCount() )
					warpSelected = 0;//go back to start if at end
					break;
		case 9: if(p_tileManager->Get_WarpCount() > 0)
					p_tileManager->Delete_Warp(warpSelected);
					break;
		}
}

void C_MapEditor::BG_ObjectsMenu()
{m_numOfRows = 9;//row count for main menu
	
	NavigateMenus();
	
	//Bounds check
	if(m_bgSelected >= BackgroundManager.ActiveSize())//go back to start if reached the end
		m_bgSelected = 0;
	if(m_bgSelected < 0)//go back to end if past the start
		m_bgSelected = BackgroundManager.ActiveSize() - 1;

	if(m_bgTypeShowing >= BackgroundManager.ListSize())//go back to start if reached the end
		m_bgTypeShowing = 0;
	if(m_bgTypeShowing < 0)//go back to end if past the start
		m_bgTypeShowing = BackgroundManager.ListSize() - 1;


	//Always highlight BG object of interest
	BackgroundManager.Set_highlight(m_bgSelected, ON);

	std::string row7String = "OFF";
	if(!m_trackPlayer){
		row7String = "ON";
		m_playerX = *Camera.Get_X();
		m_playerY = *Camera.Get_Y();
	}

	//Display Menu Options
	row1 << "BACKGROUND OBJECT MENU";//just a header
	row2 << "ADD OBJECT";
	row3 << "Toggle Layer - current: " << BackgroundManager.Get_layer(m_bgSelected);
	row4 << "Total on map: " << BackgroundManager.ActiveSize();
	row5 << "Toggle Object selected: " << m_bgSelected;
	row6 << "Delete selected Object ";
	row7 << "Track Selected:  " << row7String;

	
	row9 << "GO BACK";

	Font.Print(&row1, screen, 12, menuPosition.x, menuPosition.y + 0 );
	Font.Print(&row2, screen, 12, menuPosition.x, menuPosition.y + 16);
	Font.Print(&row3, screen, 12, menuPosition.x, menuPosition.y + 32);
	Font.Print(&row4, screen, 12, menuPosition.x, menuPosition.y + 48);
	Font.Print(&row5, screen, 12, menuPosition.x, menuPosition.y + 64);
	Font.Print(&row6, screen, 12, menuPosition.x, menuPosition.y + 80);
	Font.Print(&row7, screen, 12, menuPosition.x, menuPosition.y + 96);
	Font.Print(&row8, screen, 12, menuPosition.x, menuPosition.y + 112);
	Font.Print(&row9, screen, 12, menuPosition.x, menuPosition.y + 128);
	//If Tracking is turned on move camera to object
	if(!m_trackPlayer){
		m_tempTrackX = BackgroundManager.Get_position(m_bgSelected).x;
		m_tempTrackY = BackgroundManager.Get_position(m_bgSelected).y;
		
		
		if(m_tempTrackX > *Camera.Get_X()){//IF OBJECT IS TO THE RIGHT
			m_playerX += m_trackToX;
			Camera.SetCamera(&m_playerX, &m_playerY);
		}
		if(m_tempTrackX < *Camera.Get_X()){//IF OBJECT IS TO THE LEFT
			m_playerX -= m_trackToX;
			Camera.SetCamera(&m_playerX, &m_playerY);
		}
		if(m_tempTrackY < *Camera.Get_Y()){//IF OBJECT IS UP
			m_playerY -= m_trackToY;
			Camera.SetCamera(&m_playerX, &m_playerY );
		}
		if(m_tempTrackY > *Camera.Get_Y()){//IF OBJECT IS DOWN
			m_playerY += m_trackToY;
			Camera.SetCamera(&m_playerX, &m_playerY);
		}


		m_trackToX++;
		m_trackToY++;
	}
	else//Else we go back to tracking the player
		Camera.SetCamera((*pv_Entities)[0]->Get_p_X(), (*pv_Entities)[0]->Get_p_Y());

	//Get offsets to create objects
	SDL_Rect mouseOffset ={m_mouseX - Camera.Get_XYWH().x, m_mouseY - Camera.Get_XYWH().y};
	BackgroundManager.RenderList(m_bgTypeShowing, mouseOffset);

	if(Controls.enter || Controls.leftClick || Controls.rightClick)
		switch(m_rowSelected){
			case 2: BackgroundManager.Create(m_bgTypeShowing, BackgroundManager.Get_layer(m_bgSelected), mouseOffset);//creates object using ( TYPE, LAYER, MOUSEPOSITION)
						break;
			case 3: if(Controls.leftClick || Controls.enter)
						BackgroundManager.Set_layer(m_bgSelected, BackgroundManager.Get_layer(m_bgSelected) + 1);//increment layer by 1
					if(Controls.rightClick)
						BackgroundManager.Set_layer(m_bgSelected, BackgroundManager.Get_layer(m_bgSelected) - 1);//decrement layer by 1					
					break;
			case 5: BackgroundManager.Set_highlight(m_bgSelected, OFF);//turn highlight off for the old selection
					m_trackToX = 0;
					m_trackToY = 0;
					if(Controls.leftClick)
						m_bgSelected++;
					if(Controls.rightClick)
						m_bgSelected--;
					break;
			case 6: BackgroundManager.Remove(m_bgSelected);
					break;
			case 7: m_trackPlayer = !m_trackPlayer;//toggle row 7 ON/OFF
					break;
			case 9: m_currentMenu = MENU_MAIN;				
					break;
		}
}

void C_MapEditor::NavigateMenus()
{
	//Keep menu visible
	if(m_menuX > SCREEN_WIDTH - MENU_WIDTH)//too far right
		m_menuX = SCREEN_WIDTH - MENU_WIDTH;
	if(m_menuY > SCREEN_HEIGHT - MENU_HEIGHT)//too low
		m_menuY = SCREEN_HEIGHT - MENU_HEIGHT;
	if(m_menuY < 0)//too high
		m_menuY = 0;

	//draw background of menu
	menuPosition.x = m_menuX + 10;
	menuPosition.y = m_menuY;
	menuPosition.w = m_menuX + 300;
	menuPosition.h = m_menuY + 16*m_numOfRows;
	boxRGBA(screen, menuPosition.x, menuPosition.y, menuPosition.w, menuPosition.h, 0, 0, 0, 200); 

	//Navigation of menus IF none of the rows are locked
	if(lock_row1 && lock_row2 && lock_row3 && lock_row4 && lock_row5 && lock_row6 && lock_row7 && lock_row8 && lock_row9){
		if(Controls.wheelUp || Controls.up)
			m_rowSelected -= 1;
		if(Controls.wheelDown || Controls.down)
			m_rowSelected += 1;
	}

	if(m_rowSelected > m_numOfRows)
		m_rowSelected = 1;
	if(m_rowSelected < 1)
		m_rowSelected = m_numOfRows;

	//menuPosition.y += 16 * m_rowSelected;
	//menuPosition.h = menuPosition.y - 16;
	boxRGBA(screen, menuPosition.x, menuPosition.y + 16 * (m_rowSelected - 1), menuPosition.w, menuPosition.y + 16 * m_rowSelected, 255, 0, 0, 150);
}

void C_MapEditor::WorldMapNav()
{
	int x = (m_mouseX - Camera.Get_XYWH().x) - p_GUI->Get_mapPanX();///80 * 80;
	int y = (m_mouseY - Camera.Get_XYWH().y) - p_GUI->Get_mapPanY();///60 * 60;

	int posx = x%int(160 * p_GUI->Get_zoomAxisX()*2) * 8; //should give player x/y position within zone
	int posy = y%int(120 * p_GUI->Get_zoomAxisY()*2) * 8; //should give player x/y position within zone

	x = x / (160 * p_GUI->Get_zoomAxisX());//calc which zone is highlighted (x , y)
	y = y / (120 * p_GUI->Get_zoomAxisY());

	int mapX = (x + 2)/2;//store the selected zone
	int mapY = (y + 2)/2;	

	x *= (80 * p_GUI->Get_zoomAxisX()*2);//calculate new offset after a zoom
	y *= (60 * p_GUI->Get_zoomAxisY()*2);

	x +=  + p_GUI->Get_mapPanX();
	y +=  + p_GUI->Get_mapPanY();
	
	
	boxRGBA(screen, x, y, x + (80 * p_GUI->Get_zoomAxisX()*2), y + (60 * p_GUI->Get_zoomAxisY()*2), 255, 201 , 0, m_mapAlphaCounter);

	m_mapAlphaCounter += 3;

	if(m_mapAlphaCounter > 200)
		m_mapAlphaCounter = 50;


	if(Controls.leftClick && mapMode == ON){
	//MAGIC
	//create a warp zone on player with coordinates of area clicked on
	SDL_Rect playerCoordinate = { (*pv_Entities)[0]->Get_X(), (*pv_Entities)[0]->Get_Y()};
	SDL_Rect warpingToThisMap = {mapX, mapY};
	SDL_Rect toThisLocation = {posx, posy};
	p_tileManager->Create_Warp();
	p_tileManager->Set_WarpPosition(0, playerCoordinate);//position warp on player
	p_tileManager->Set_WarpEndMap(0, warpingToThisMap); //warp to wherever player clicked
	p_tileManager->Set_WarpEndPosition(0, toThisLocation);//to this specific spot
	p_tileManager->Set_WarpType(0, OVERWORLD);//force overworld only for now

	mapMode = OFF; //turn off map mode and warp
	MapEditorMenuActive = OFF;
	MapEditorActive = OFF; //same & warp
	Controls.leftClick = OFF;//this sticks?
	}
}
#endif