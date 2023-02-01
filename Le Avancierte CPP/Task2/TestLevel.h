//Inkludera först i alla cpp filer 
#pragma once

//Memory leaks
#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#ifndef DBG_NEW
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#define LEVEL 14
#define G_BETYG
#define VG_BETYG

//Börja med att ha G_BETYG och VG_BETYG undefined och sätt LEVEL till 1.
// Stega sedan upp LEVEL till 8
// Definiera G_BETYG, då körs även TestSharedG()
// Om du satsar på G så är det klart nu annars
// Stega sedan upp LEVEL till ungefär 15
// sätt LEVEL till 99
// Definiera VG_BETYG, nu då körs alla tester.
// 
// 
//Observera att LEVEL testerna är mycket enkla
//det är först G_BETYG resp. VG_BETYG som blir en ordentlig test.

/* Nivåerna man kan sätta på LEVEL
#if false
level 1
SharedPtr();
SharedPtr(T*);
get()
used_count()
Invariant();

level 2
SharedPtr(nullptr);
SharedPtr(const SharedPtr&);


level 3
operator*
operator->
operator bool


level 4
    SharedPtr(const SharedPtr&&)
    operator=(const SharedPtr&&)
    reset
Level 5
==, <  allihop

level 6
swap

level 7
MakeShared


//Nu börjar VG
level 11
WeakPtr();
~WeakPtr();
expired();
Invariant();
    
level 12
WeakPtr(const SharedPtr&);
Lock()

level 13
WeakPtr(const WeakPtr&);
operator=(const WeakPtr&);
operator=(const SharedPtr&);

level 14
    reset
    swap

#endif


*/