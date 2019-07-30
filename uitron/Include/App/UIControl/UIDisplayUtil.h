#ifndef UIDISPLAYUTIL_H
#define UIDISPLAYUTIL_H
#ifdef NULL
#undef NULL
#define NULL 0
#endif
extern VControl NULLCtrl;
//////////////////////////////////////////////////////////////////////////////
#define NULLCmdMap  NULL
#define NULLShow    NULL

//#NT#2008/09/18#lincy-begin
#define SHOW_ITEM_RECT(type,name,x1,y1,x2,y2,lineStl,fillStl,fClr,bgClr,layout,align,map)  \
	static ITEM_RECTANGLE   name = {(type), x1,y1,x2,y2,lineStl,fillStl,fClr,bgClr,layout,align,(void *)map};
#define SHOW_ITEM_ROUNDRECT(type,name,x1,y1,x2,y2,lineStl,fillStl,fClr,bgClr,layout,align,map,rx,ry)  \
	static ITEM_ROUNDRECT   name = {(type), x1,y1,x2,y2,lineStl,fillStl,fClr,bgClr,layout,align,(void *)map,rx,ry};
#define SHOW_ITEM_ELLIPSE(type,name,x1,y1,x2,y2,lineStl,fillStl,fClr,bgClr,layout,align,map)  \
	static ITEM_ELLIPSE   name = {(type), x1,y1,x2,y2,lineStl,fillStl,fClr,bgClr,layout,align,(void *)map};
#define SHOW_ITEM_LINE(type,name,x1,y1,x2,y2,lineStl,fillStl,lineClr,map)  \
	static ITEM_LINE   name = {(type), x1,y1,x2,y2,lineStl,fillStl,lineClr,(void *)map};
#define SHOW_ITEM_IMAGE(type,name,x1,y1,x2,y2,data,bltRop,ParRop,clrkey,clrMap,layout,clip,align,imageT)  \
	static ITEM_IMAGE  name = {(type), x1,y1,x2,y2,data,bltRop,ParRop,clrkey,(void *)clrMap,layout,clip,align,(void *)imageT};
#define SHOW_ITEM_TEXT(type,name,x1,y1,x2,y2,data,fontT,fontstl,fillstl,fontsize,tclr,ShClr,LiClr,layout,clip,align,lineH,LeSpe,InSpe,StrT)  \
	static ITEM_TEXT   name = {(type), x1,y1,x2,y2,data,(void *)fontT,fontstl,fillstl,fontsize,tclr,ShClr,LiClr,layout,clip,align,lineH,LeSpe,InSpe,(void *)StrT};
#define SHOW_ITEM_GROUP(type,name,x1,y1,x2,y2)  \
	static ITEM_GROUP   name = {(type), x1,y1,x2,y2,(ITEM_BASE**)(name##Show)};

//////////////////////////////////////////////////////////////////////////

#define SHOW_MAP_BEGIN(Name) static ITEM_BASE* Name##Show[]={
#define SHOW_MAP_BODY(Name)  (ITEM_BASE*)&Name,
#define SHOW_MAP_END         NULL};
//#NT#2008/09/18#lincy-end

//////////////////////////////////////////////////////////////////////////
#define DECLARE_EVENT(Name)  extern EVENT_ENTRY Name##CmdMap[];
#define EVENT_BEGIN(Name) EVENT_ENTRY Name##CmdMap[]={
#define EVENT_ITEM(Event,pfn) {Event,pfn},
#define EVENT_END {NVTEVT_NULL,0},};
///////////////////////////////////////////////////////////////////////////////////
#define CREATE_ZOOM_DATA(Name,value)             \
	CTRL_ZOOM_DATA Name##Data={                                        \
																	   value                                                            \
							  };
///////////////////////////////////////////////////////////////////////////////////
#define CREATE_STATIC_DATA(Name,value)             \
	CTRL_STATIC_DATA Name##Data={                                        \
																		 value                                                            \
								};
///////////////////////////////////////////////////////////////////////////////////
#define CREATE_STATE_ITEM_DATA(Name,stringID,iconID,pExeFunc)  \
	static CTRL_STATE_ITEM_DATA Name##Item={                                             \
																						 stringID,                                                           \
																						 iconID,                                                             \
																						 pExeFunc                                                  \
										   };

#define CREATE_STATE_DATA(Name,currentItem)             \
	CTRL_STATE_DATA Name##Data={                                        \
																		currentItem,                                                   \
																		sizeof(Name##StateList)/sizeof(CTRL_STATE_ITEM_DATA *) -1,          \
																		Name##StateList,                                                \
							   };
//state graph total change -2 because first group is skin,not a state
#define CREATE_STATEGRAPH_DATA(Name,status,pFunc)       \
	CTRL_STATEGRAPH_DATA Name##Data={                                        \
																			 status,                                                                             \
																			 pFunc,                                                                              \
																			 sizeof(Name##Show)/sizeof(ITEM_BASE*) -2,                      \
									};

#define CREATE_PANEL_DATA(Name,status,pFunc)        \
	CTRL_TAB_DATA Name##Data={                          \
														status,                                         \
														pFunc                                           \
							 };

#define CREATE_TAB_DATA(Name,focus)                 \
	CTRL_TAB_DATA Name##Data={                          \
														focus,                                          \
														sizeof(Name##CtrlList)/sizeof(VControl *) -1,   \
							 };

#define CREATE_BUTTON_ITEM_DATA(Name,stringID,iconID,StatusFlag,pExeFunc)  \
	static CTRL_BUTTON_ITEM_DATA Name##Item={                                             \
																						  stringID,                                                           \
																						  iconID,                                                             \
																						  StatusFlag,                                                          \
																						  pExeFunc,                                                            \
											};

#define CREATE_BUTTON_DATA(Name,style)       \
	CTRL_BUTTON_DATA Name##Data={                                        \
																		 style,                                                   \
																		 Name##ButtonList,                                                \
								};


#define CREATE_MENU_ITEM_DATA(Name,stringID,iconID,StatusFlag,pExeFunc,value)  \
	static CTRL_MENU_ITEM_DATA Name##Item={                                             \
																						stringID,                                                           \
																						iconID,                                                             \
																						StatusFlag,                                                          \
																						pExeFunc,                                                            \
																						NULL,                                                           \
																						value,                                                                  \
										  };

#define CREATE_MENU_DATA(Name,currentItem,pageItem,style)           \
	CTRL_MENU_DATA Name##Data={                                        \
																	   currentItem,                                                   \
																	   sizeof(Name##MenuList)/sizeof(CTRL_MENU_ITEM_DATA *) -1,          \
																	   pageItem,                                                   \
																	   style,                                                      \
																	   0,                                                             \
																	   Name##MenuList,                                                \
																	   sizeof(Name##MenuList)/sizeof(CTRL_MENU_ITEM_DATA *) -1,          \
							  };

#define CREATE_LIST_ITEM_DATA(Name,stringID,iconID,StatusFlag)  \
	static CTRL_LIST_ITEM_DATA Name##Item={                                             \
																						stringID,                                                           \
																						iconID,                                                             \
																						StatusFlag,                                                          \
										  };

#define CREATE_LIST_DATA(Name,currentItem,pageItem,style,event)           \
	CTRL_LIST_DATA Name##Data={                                        \
																	   currentItem,                                                   \
																	   sizeof(Name##ListList)/sizeof(CTRL_LIST_ITEM_DATA *) -1,          \
																	   pageItem,                                                   \
																	   style,                                                      \
																	   0,                                                             \
																	   Name##ListList,                                                \
																	   sizeof(Name##ListList)/sizeof(CTRL_LIST_ITEM_DATA *) -1,          \
																	   event,                                                         \
							  };

#define CREATE_SLIDER_DATA(Name,sliderType,currentItem,totalItem)      \
	CTRL_SLIDER_DATA Name##Data={                               \
																(sliderType),                                           \
																currentItem,                                            \
																totalItem,                                              \
								};

#define CREATE_SCRBAR_DATA(Name,scrlbarType,scrlbarShape,currentItem,totalItem)  \
	CTRL_SCRBAR_DATA Name##Data={                               \
																(scrlbarType),                                          \
																scrlbarShape,                                           \
																currentItem,                                            \
																totalItem,                                              \
																0,                                                      \
								};

#define CREATE_PROBAR_DATA(Name,progressbarType,currentItem,totalItem)  \
	CTRL_PROBAR_DATA Name##Data={                               \
																(progressbarType),                                      \
																currentItem,                                            \
																totalItem,                                              \
								};

//////////////////////////////////////////////////////////////////////////////////
#define LIST_LIST_BEGIN(Name)  CTRL_LIST_ITEM_DATA *Name##ListList[]={
#define LIST_LIST_ITEM(Name)         &Name##Item,
#define LIST_LIST_END           0};

//////////////////////////////////////////////////////////////////////////////////
#define STATE_LIST_BEGIN(Name)  CTRL_STATE_ITEM_DATA *Name##StateList[]={
#define STATE_LIST_ITEM(Name)         &Name##Item,
#define STATE_LIST_END           0};

//////////////////////////////////////////////////////////////////////////////////
#define BUTTON_LIST_BEGIN(Name)  CTRL_BUTTON_ITEM_DATA *Name##ButtonList[]={
#define BUTTON_LIST_ITEM(Name)         &Name##Item,
#define BUTTON_LIST_END           0};

//////////////////////////////////////////////////////////////////////////////////
#define MENU_LIST_BEGIN(Name)  CTRL_MENU_ITEM_DATA *Name##MenuList[]={
#define MENU_LIST_ITEM(Name)         &Name##Item,
#define MENU_LIST_END           0};

//////////////////////////////////////////////////////////////////////////////////
#define DECLARE_CTRL_LIST(Name)       extern VControl *Name##CtrlList[];
#define CTRL_LIST_BEGIN(Name)   VControl *Name##CtrlList[]={
#define CTRL_LIST_ITEM(Name)         &Name##Ctrl,
#define CTRL_LIST_END              0};

///////////////////////////////////////////////////////////////////////////////
#define CREATE_CTRL(Name,ShowTable,Type,Data,Parent,x1,y1,x2,y2)        \
	VControl Name##Ctrl={ \
						  #Name,            \
						  Type,             \
						  Name##CtrlList,   \
						  Parent,           \
						  0,                \
						  Name##CmdMap,     \
						  Data,             \
						  ShowTable##Show,  \
						  {(x1),(y1),(x2),(y2)},    \
						  0                 \
						};

#define CREATE_PANEL_CTRL(Name,Parent,x1,y1,x2,y2)        \
	VControl Name##Ctrl={ \
						  #Name,            \
						  CTRL_PANEL,       \
						  Name##CtrlList,   \
						  Parent,           \
						  0,                \
						  NULL,             \
						  (void *)&Name##Data,    \
						  NULL,             \
						  {(x1),(y1),(x2),(y2)},    \
						  0                 \
						};

#define CREATE_TAB_CTRL(Name,Cmd,Parent,x1,y1,x2,y2)        \
	VControl Name##Ctrl={       \
								#Name,                  \
								CTRL_TAB,               \
								Name##CtrlList,         \
								Parent,                 \
								0,                      \
								Cmd##CmdMap,            \
								(void *)&Name##Data,    \
								Name##Show,             \
								{(x1),(y1),(x2),(y2)},  \
								0                       \
						};

#define CREATE_ZOOM_CTRL(Name,Parent,x1,y1,x2,y2)        \
	VControl Name##Ctrl={       \
								#Name,                  \
								CTRL_ZOOM,              \
								NULL,                   \
								Parent,                 \
								0,                      \
								Name##CmdMap,           \
								(void *)&Name##Data,    \
								Name##Show,             \
								{(x1),(y1),(x2),(y2)},  \
								0                       \
						};
#define CREATE_STATIC_CTRL(Name,Parent,x1,y1,x2,y2)        \
	VControl Name##Ctrl={       \
								#Name,                  \
								CTRL_STATIC,            \
								NULL,                   \
								Parent,                 \
								0,                       \
								Name##CmdMap,           \
								(void *)&Name##Data,    \
								Name##Show,             \
								{(x1),(y1),(x2),(y2)},  \
								0                      \
						};
#define CREATE_STATE_CTRL(Name,Parent,x1,y1,x2,y2)        \
	VControl Name##Ctrl={       \
								#Name,                  \
								CTRL_STATE,             \
								NULL,                   \
								Parent,                 \
								0,                      \
								Name##CmdMap,           \
								(void *)&Name##Data,    \
								Name##Show,             \
								{(x1),(y1),(x2),(y2)},  \
								0                       \
						};
#define CREATE_STATEGRAPH_CTRL(Name,Cmd,Parent,x1,y1,x2,y2)        \
	VControl Name##Ctrl={       \
								#Name,                  \
								CTRL_STATEGRAPH,        \
								NULL,                   \
								Parent,                 \
								0,                      \
								Cmd##CmdMap,            \
								(void *)&Name##Data,    \
								Name##Show,             \
								{(x1),(y1),(x2),(y2)},  \
								0                       \
						};
#define CREATE_BUTTON_CTRL(Name,Cmd,Parent,x1,y1,x2,y2)        \
	VControl Name##Ctrl={       \
								#Name,                  \
								CTRL_BUTTON,            \
								NULL,                   \
								Parent,                 \
								0,                      \
								Cmd##CmdMap,            \
								(void *)&Name##Data,    \
								Name##Show,             \
								{(x1),(y1),(x2),(y2)},  \
								0                       \
						};

#define CREATE_MENU_CTRL(Name,Cmd,Parent,x1,y1,x2,y2)       \
	VControl Name##Ctrl={       \
								#Name,                  \
								CTRL_MENU,              \
								NULL,                   \
								Parent,                 \
								0,                      \
								Cmd##CmdMap,            \
								(void *)&Name##Data,    \
								Name##Show,             \
								{(x1),(y1),(x2),(y2)},  \
								0                       \
						};

#define CREATE_LIST_CTRL(Name,Cmd,Parent,x1,y1,x2,y2)        \
	VControl Name##Ctrl={       \
								#Name,                  \
								CTRL_LIST,              \
								NULL,                   \
								Parent,                 \
								0,                      \
								Cmd##CmdMap,            \
								(void *)&Name##Data,    \
								Name##Show,             \
								{(x1),(y1),(x2),(y2)},  \
								0                       \
						};

#define CREATE_SLIDER_CTRL(Name,Cmd,Parent,x1,y1,x2,y2)        \
	VControl Name##Ctrl={       \
								#Name,                  \
								CTRL_SLIDER,            \
								Name##CtrlList,         \
								Parent,                 \
								0,                      \
								Cmd##CmdMap,            \
								(void *)&Name##Data,    \
								Name##Show,             \
								{(x1),(y1),(x2),(y2)},  \
								0                       \
						};

#define CREATE_SCRBAR_CTRL(Name,Cmd,Parent,x1,y1,x2,y2)       \
	VControl Name##Ctrl={       \
								#Name,                  \
								CTRL_SCROLLBAR,         \
								Name##CtrlList,         \
								Parent,                 \
								0,                      \
								Cmd##CmdMap,            \
								(void *)&Name##Data,    \
								Name##Show,             \
								{(x1),(y1),(x2),(y2)},  \
								0                       \
						};

#define CREATE_PROBAR_CTRL(Name,Cmd,Parent,x1,y1,x2,y2)        \
	VControl Name##Ctrl={       \
								#Name,                  \
								CTRL_PROGRESSBAR,       \
								Name##CtrlList,         \
								Parent,                 \
								0,                      \
								Cmd##CmdMap,            \
								(void *)&Name##Data,    \
								Name##Show,             \
								{(x1),(y1),(x2),(y2)},  \
								0                       \
						};

/*********************************************
marcro usage
#define Conn(x,y) x##y
#define ToChar(x) #@x
#define ToString(x) #x
**********************************************/

#endif
