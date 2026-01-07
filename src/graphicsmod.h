

#ifndef GRAPHICSMOD_DATA_H
#define GRAPHICSMOD_DATA_H

#include <sys/types.h>
#include "speccypal.h"

// Graphics modifiers
#define GM_NO_CHANGE 	0b00000000
#define FLIP_HORIZONTAL 0b00000001
#define FLIP_VERTICAL 	0b00000010

struct graphics_modifier 
{
	const __uint8_t index;
	const __uint8_t graphic_modifiers; // e.g., FLIP_HORIZONTAL, FLIP_VERTICAL
	const __uint8_t srcimage_data;
	const __uint8_t color_modifiers[4]; // up to 4 color modifications 0 =  means no change
    const __uint8_t padding; // align to 8 bytes      
};

    
const struct graphics_modifier graphics_modifiers[] = {

	// Items: 133, will be extra  51072 bytes in game memory

	{	// Garden fence magenta 
		100, GM_NO_CHANGE, 9,  // from: Garden fence white  
		{ COLOR_WHITE << 4 | COLOR_MAGENTA, 0, 0, 0,} },

	{	// Garden fence yellow 
		101, GM_NO_CHANGE, 9,  // from: Garden fence white  
		{ COLOR_WHITE << 4 | COLOR_YELLOW, 0, 0, 0,} },

	{	// Fountain white bottom right 
		102, GM_NO_CHANGE, 28,  // from: Fountain magenta bottom right 
		{ COLOR_MAGENTA << 4 | COLOR_WHITE, COLOR_DARK_MAGENTA << 4 | COLOR_GREY, 0, 0,} },

	{	// Fountain white bottom left 
		103, GM_NO_CHANGE, 29,  // from: Fountain magenta bottom left 
		{ COLOR_MAGENTA << 4 | COLOR_WHITE, COLOR_DARK_MAGENTA << 4 | COLOR_GREY, 0, 0,} },

	{	// Fountain white top right 
		104, GM_NO_CHANGE, 30,  // from: Fountain magenta top right 
		{ COLOR_MAGENTA << 4 | COLOR_WHITE, COLOR_DARK_MAGENTA << 4 | COLOR_GREY, 0, 0,} },

	{	// Fountain white top left 
		105, GM_NO_CHANGE, 31,  // from: Fountain magenta top left 
		{ COLOR_MAGENTA << 4 | COLOR_WHITE, COLOR_DARK_MAGENTA << 4 | COLOR_GREY, 0, 0,} },

	{	// Fountain cyan bottom right 
		106, GM_NO_CHANGE, 28,  // from: Fountain magenta bottom right 
		{ COLOR_MAGENTA << 4 | COLOR_CYAN, COLOR_DARK_MAGENTA << 4 | COLOR_GREY, 0, 0,} },

	{	// Fountain cyan bottom left 
		107, GM_NO_CHANGE, 29,  // from: Fountain magenta bottom left 
		{ COLOR_MAGENTA << 4 | COLOR_CYAN, COLOR_DARK_MAGENTA << 4 | COLOR_GREY, 0, 0,} },

	{	// Fountain cyan top right 
		108, GM_NO_CHANGE, 30,  // from: Fountain magenta top right 
		{ COLOR_MAGENTA << 4 | COLOR_CYAN, COLOR_DARK_MAGENTA << 4 | COLOR_GREY, 0, 0,} },

	{	// Fountain cyan top left 
		109, GM_NO_CHANGE, 31,  // from: Fountain magenta top left 
		{ COLOR_MAGENTA << 4 | COLOR_CYAN, COLOR_DARK_MAGENTA << 4 | COLOR_GREY, 0, 0,} },

	{	// Fountain yellow bottom right 
		110, GM_NO_CHANGE, 28,  // from: Fountain magenta bottom right 
		{ COLOR_MAGENTA << 4 | COLOR_YELLOW, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_YELLOW, 0, 0,} },

	{	// Fountain yellow bottom left 
		111, GM_NO_CHANGE, 29,  // from: Fountain magenta bottom left 
		{ COLOR_MAGENTA << 4 | COLOR_YELLOW, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_YELLOW, 0, 0,} },

	{	// Fountain yellow top right 
		112, GM_NO_CHANGE, 30,  // from: Fountain magenta top right 
		{ COLOR_MAGENTA << 4 | COLOR_YELLOW, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_YELLOW, 0, 0,} },

	{	// Fountain yellow top left 
		113, GM_NO_CHANGE, 31,  // from: Fountain magenta top left 
		{ COLOR_MAGENTA << 4 | COLOR_YELLOW, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_YELLOW, 0, 0,} },

	{	// Fountain green bottom right 
		114, GM_NO_CHANGE, 28,  // from: Fountain magenta bottom right 
		{ COLOR_MAGENTA << 4 | COLOR_GREEN, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_GREEN, 0, 0,} },

	{	// Fountain green bottom left 
		115, GM_NO_CHANGE, 29,  // from: Fountain magenta bottom left 
		{ COLOR_MAGENTA << 4 | COLOR_GREEN, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_GREEN, 0, 0,} },

	{	// Fountain green top right 
		116, GM_NO_CHANGE, 30,  // from: Fountain magenta top right 
		{ COLOR_MAGENTA << 4 | COLOR_GREEN, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_GREEN, 0, 0,} },

	{	// Fountain green top left 
		117, GM_NO_CHANGE, 31,  // from: Fountain magenta top left 
		{ COLOR_MAGENTA << 4 | COLOR_GREEN, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_GREEN, 0, 0,} },

	{	// Flower 2 white magenta 
		118, GM_NO_CHANGE, 19,  // from: Flower 2 magenta yellow 
		{ COLOR_MAGENTA << 4 | COLOR_WHITE, COLOR_YELLOW << 4 | COLOR_MAGENTA, 0, 0,} },

	{	// Flower 2 light_cyan white 
		119, GM_NO_CHANGE, 19,  // from: Flower 2 magenta yellow 
		{ COLOR_MAGENTA << 4 | COLOR_LIGHT_CYAN, COLOR_YELLOW << 4 | COLOR_WHITE, 0, 0,} },

	{	// Flower 2 white red 
		120, GM_NO_CHANGE, 19,  // from: Flower 2 magenta yellow 
		{ COLOR_MAGENTA << 4 | COLOR_WHITE, COLOR_YELLOW << 4 | COLOR_RED, 0, 0,} },

	{	// Flower 2 yellow dark_magenta 
		121, GM_NO_CHANGE, 19,  // from: Flower 2 magenta yellow 
		{ COLOR_YELLOW << 4 | COLOR_DARK_MAGENTA, COLOR_MAGENTA << 4 | COLOR_YELLOW, 0, 0,} },

	{	// Flower 1 cyan yellow 
		122, GM_NO_CHANGE, 18,  // from: Flower 1 cyan white 
		{ COLOR_WHITE << 4 | COLOR_YELLOW, COLOR_LIGHT_CYAN << 4 | COLOR_MAGENTA, 0, 0,} },

	{	// Flower 1 cyan magenta 
		123, GM_NO_CHANGE, 18,  // from: Flower 1 cyan white 
		{ COLOR_WHITE << 4 | COLOR_MAGENTA, COLOR_LIGHT_CYAN << 4 | COLOR_WHITE, 0, 0,} },

	{	// Flower 2 magenta white 
		124, GM_NO_CHANGE, 19,  // from: Flower 2 magenta yellow 
		{ COLOR_YELLOW << 4 | COLOR_WHITE, 0, 0, 0,} },

	{	// Flower 2 white grey 
		125, GM_NO_CHANGE, 19,  // from: Flower 2 magenta yellow 
		{ COLOR_YELLOW << 4 | COLOR_GREY, COLOR_MAGENTA << 4 | COLOR_WHITE, 0, 0,} },

	{	// Plantpot red 
		126, GM_NO_CHANGE, 27,  // from: Plantpot white  
		{ COLOR_WHITE << 4 | COLOR_RED, COLOR_GREY << 4 | COLOR_DARK_RED, 0, 0,} },

	{	// Garden fence light_cyan 
		127, GM_NO_CHANGE, 9,  // from: Garden fence white  
		{ COLOR_WHITE << 4 | COLOR_LIGHT_CYAN, 0, 0, 0,} },

	{	// Flower 1 cyan white 
		128, GM_NO_CHANGE, 18,  // from: Flower 1 cyan white 
		{ COLOR_LIGHT_CYAN << 4 | COLOR_MAGENTA, 0, 0, 0,} },

	{	// Flower 1 cyan magenta 
		129, GM_NO_CHANGE, 18,  // from: Flower 1 cyan white 
		{ COLOR_WHITE << 4 | COLOR_MAGENTA, COLOR_LIGHT_CYAN << 4 | COLOR_YELLOW, 0, 0,} },

	{	// Tiling symbol black light_cyan 
		130, GM_NO_CHANGE, 11,  // from: Tiling symbol white blue 
		{ COLOR_WHITE << 4 | COLOR_BLACK, COLOR_BLUE << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_BLUE << 4 | COLOR_CYAN, 0,} },

	{	// Tiling symbol black yellow 
		131, GM_NO_CHANGE, 11,  // from: Tiling symbol white blue 
		{ COLOR_WHITE << 4 | COLOR_BLACK, COLOR_BLUE << 4 | COLOR_YELLOW, COLOR_DARK_BLUE << 4 | COLOR_DARK_YELLOW, 0,} },

	{	// Tiling symbol black green 
		132, GM_NO_CHANGE, 11,  // from: Tiling symbol white blue 
		{ COLOR_WHITE << 4 | COLOR_BLACK, COLOR_BLUE << 4 | COLOR_GREEN, COLOR_DARK_BLUE << 4 | COLOR_DARK_GREEN, 0,} },

	{	// Tiling symbol red white 
		133, GM_NO_CHANGE, 11,  // from: Tiling symbol white blue 
		{ COLOR_WHITE << 4 | COLOR_RED, COLOR_GREY << 4 | COLOR_DARK_RED, COLOR_BLUE << 4 | COLOR_WHITE, COLOR_DARK_BLUE << 4 | COLOR_GREY,} },

	{	// Garden fence red 
		134, GM_NO_CHANGE, 9,  // from: Garden fence white  
		{ COLOR_WHITE << 4 | COLOR_RED, 0, 0, 0,} },

	{	// Tiling cyan 
		135, GM_NO_CHANGE, 10,  // from: Tiling cyan   
		{ COLOR_WHITE << 4 | COLOR_YELLOW, COLOR_LIGHT_CYAN << 4 | COLOR_WHITE, COLOR_CYAN << 4 | COLOR_WHITE, 0,} },

	{	// Tiling cyan 
		136, GM_NO_CHANGE, 10,  // from: Tiling cyan   
		{ COLOR_WHITE << 4 | COLOR_MAGENTA, COLOR_DARK_BLUE << 4 | COLOR_WHITE, COLOR_LIGHT_CYAN << 4 | COLOR_RED, COLOR_CYAN << 4 | COLOR_DARK_RED,} },

	{	// Tiling cyan 
		137, GM_NO_CHANGE, 10,  // from: Tiling cyan   
		{ COLOR_WHITE << 4 | COLOR_RED, COLOR_DARK_BLUE << 4 | COLOR_WHITE, COLOR_LIGHT_CYAN << 4 | COLOR_DARK_RED, COLOR_CYAN << 4 | COLOR_WHITE,} },

	{	// Tiling cyan 
		138, GM_NO_CHANGE, 10,  // from: Tiling cyan   
		{ COLOR_DARK_BLUE << 4 | COLOR_GREY, COLOR_LIGHT_CYAN << 4 | COLOR_MAGENTA, COLOR_CYAN << 4 | COLOR_DARK_MAGENTA, 0,} },

	{	// Tiling cyan 
		139, GM_NO_CHANGE, 10,  // from: Tiling cyan   
		{ COLOR_WHITE << 4 | COLOR_YELLOW, COLOR_LIGHT_CYAN << 4 | COLOR_WHITE, COLOR_CYAN << 4 | COLOR_GREEN, 0,} },

	{	// Tiling cyan 
		140, GM_NO_CHANGE, 10,  // from: Tiling cyan   
		{ COLOR_LIGHT_CYAN << 4 | COLOR_YELLOW, COLOR_CYAN << 4 | COLOR_DARK_YELLOW, 0, 0,} },

	{	// Tiling cyan 
		141, GM_NO_CHANGE, 10,  // from: Tiling cyan   
		{ COLOR_DARK_BLUE << 4 | COLOR_WHITE, 0, 0, 0,} },

	{	// Tiling cyan 
		142, GM_NO_CHANGE, 10,  // from: Tiling cyan   
		{ COLOR_CYAN << 4 | COLOR_DARK_GREEN, COLOR_LIGHT_CYAN << 4 | COLOR_GREEN, 0, 0,} },

	{	// Key magenta 
		143, GM_NO_CHANGE, 46,  // from: Key green  
		{ COLOR_GREEN << 4 | COLOR_MAGENTA, COLOR_DARK_GREEN << 4 | COLOR_DARK_MAGENTA, 0, 0,} },

	{	// Crab a magenta 
		144, GM_NO_CHANGE, 37,  // from: Crab a magenta 
		{ COLOR_DARK_MAGENTA << 4 | COLOR_RED, 0, 0, 0,} },

	{	// Crab b magenta 
		145, GM_NO_CHANGE, 38,  // from: Crab b magenta 
		{ COLOR_DARK_MAGENTA << 4 | COLOR_RED, 0, 0, 0,} },

	{	// Snake head right 
		146, GM_NO_CHANGE, 65,  // from: Snake head right 
		{ COLOR_MAGENTA << 4 | COLOR_RED, COLOR_GREEN << 4 | COLOR_MAGENTA, COLOR_DARK_GREEN << 4 | COLOR_DARK_MAGENTA, 0,} },

	{	// Snake head left 
		147, GM_NO_CHANGE, 75,  // from: Snake head left 
		{ COLOR_MAGENTA << 4 | COLOR_RED, COLOR_GREEN << 4 | COLOR_MAGENTA, COLOR_DARK_GREEN << 4 | COLOR_DARK_MAGENTA, 0,} },

	{	// Snake head right 
		148, GM_NO_CHANGE, 65,  // from: Snake head right 
		{ COLOR_GREEN << 4 | COLOR_YELLOW, COLOR_DARK_GREEN << 4 | COLOR_DARK_YELLOW, 0, 0,} },

	{	// Snake head left 
		149, GM_NO_CHANGE, 75,  // from: Snake head left 
		{ COLOR_GREEN << 4 | COLOR_YELLOW, COLOR_DARK_GREEN << 4 | COLOR_DARK_YELLOW, 0, 0,} },

	{	// Carpet light_cyan bottom left 
		150, GM_NO_CHANGE, 32,  // from: Carpet magenta bottom left 
		{ COLOR_MAGENTA << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_MAGENTA << 4 | COLOR_CYAN, 0, 0,} },

	{	// Carpet light_cyan top left 
		151, GM_NO_CHANGE, 33,  // from: Carpet magenta top left 
		{ COLOR_MAGENTA << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_MAGENTA << 4 | COLOR_CYAN, 0, 0,} },

	{	// Carpet light_cyan top right 
		152, GM_NO_CHANGE, 34,  // from: Carpet magenta top right 
		{ COLOR_MAGENTA << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_MAGENTA << 4 | COLOR_CYAN, 0, 0,} },

	{	// Carpet light_cyan bottom right 
		153, GM_NO_CHANGE, 35,  // from: Carpet magenta bottom right 
		{ COLOR_MAGENTA << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_MAGENTA << 4 | COLOR_CYAN, 0, 0,} },

	{	// Carpet yellow bottom left 
		155, GM_NO_CHANGE, 32,  // from: Carpet magenta bottom left 
		{ COLOR_YELLOW << 4 | COLOR_RED, COLOR_MAGENTA << 4 | COLOR_YELLOW, COLOR_DARK_MAGENTA << 4 | COLOR_YELLOW, 0,} },

	{	// Carpet yellow top left 
		156, GM_NO_CHANGE, 33,  // from: Carpet magenta top left 
		{ COLOR_YELLOW << 4 | COLOR_RED, COLOR_MAGENTA << 4 | COLOR_YELLOW, COLOR_DARK_MAGENTA << 4 | COLOR_YELLOW, 0,} },

	{	// Carpet yellow top right 
		157, GM_NO_CHANGE, 34,  // from: Carpet magenta top right 
		{ COLOR_YELLOW << 4 | COLOR_RED, COLOR_MAGENTA << 4 | COLOR_YELLOW, COLOR_DARK_MAGENTA << 4 | COLOR_YELLOW, 0,} },

	{	// Carpet yellow bottom right 
		158, GM_NO_CHANGE, 35,  // from: Carpet magenta bottom right 
		{ COLOR_YELLOW << 4 | COLOR_RED, COLOR_MAGENTA << 4 | COLOR_YELLOW, COLOR_DARK_MAGENTA << 4 | COLOR_YELLOW, 0,} },

	{	// Hedge 
		159, GM_NO_CHANGE, 20,  // from: Hedge   
		{ COLOR_GREEN << 4 | COLOR_YELLOW, COLOR_DARK_GREEN << 4 | COLOR_DARK_YELLOW, 0, 0,} },

	{	// Carpet green bottom left 
		160, GM_NO_CHANGE, 32,  // from: Carpet magenta bottom left 
		{ COLOR_YELLOW << 4 | COLOR_DARK_GREEN, COLOR_MAGENTA << 4 | COLOR_GREEN, COLOR_DARK_MAGENTA << 4 | COLOR_WHITE, 0,} },

	{	// Carpet green top left 
		161, GM_NO_CHANGE, 33,  // from: Carpet magenta top left 
		{ COLOR_YELLOW << 4 | COLOR_DARK_GREEN, COLOR_MAGENTA << 4 | COLOR_GREEN, COLOR_DARK_MAGENTA << 4 | COLOR_WHITE, 0,} },

	{	// Carpet green top right 
		162, GM_NO_CHANGE, 34,  // from: Carpet magenta top right 
		{ COLOR_YELLOW << 4 | COLOR_DARK_GREEN, COLOR_MAGENTA << 4 | COLOR_GREEN, COLOR_DARK_MAGENTA << 4 | COLOR_WHITE, 0,} },

	{	// Carpet green bottom right 
		163, GM_NO_CHANGE, 35,  // from: Carpet magenta bottom right 
		{ COLOR_YELLOW << 4 | COLOR_DARK_GREEN, COLOR_MAGENTA << 4 | COLOR_GREEN, COLOR_DARK_MAGENTA << 4 | COLOR_WHITE, 0,} },

	{	// Carpet light_cyan bottom left 
		165, GM_NO_CHANGE, 32,  // from: Carpet magenta bottom left 
		{ COLOR_MAGENTA << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_MAGENTA << 4 | COLOR_CYAN, 0, 0,} },

	{	// Carpet light_cyan top left 
		166, GM_NO_CHANGE, 33,  // from: Carpet magenta top left 
		{ COLOR_MAGENTA << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_MAGENTA << 4 | COLOR_CYAN, 0, 0,} },

	{	// Carpet light_cyan top right 
		167, GM_NO_CHANGE, 34,  // from: Carpet magenta top right 
		{ COLOR_MAGENTA << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_MAGENTA << 4 | COLOR_CYAN, 0, 0,} },

	{	// Carpet light_cyan bottom right 
		168, GM_NO_CHANGE, 35,  // from: Carpet magenta bottom right 
		{ COLOR_MAGENTA << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_MAGENTA << 4 | COLOR_CYAN, 0, 0,} },

	{	// Flowerpot light_cyan 
		169, GM_NO_CHANGE, 26,  // from: Flowerpot white  
		{ COLOR_WHITE << 4 | COLOR_LIGHT_CYAN, COLOR_GREY << 4 | COLOR_CYAN, COLOR_MAGENTA << 4 | COLOR_WHITE, COLOR_YELLOW << 4 | COLOR_RED,} },

	{	// Cushion light_cyan 
		170, GM_NO_CHANGE, 22,  // from: Cushion red  
		{ COLOR_RED << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_RED << 4 | COLOR_CYAN, COLOR_YELLOW << 4 | COLOR_WHITE, 0,} },

	{	// Cushion green 
		171, GM_NO_CHANGE, 22,  // from: Cushion red  
		{ COLOR_RED << 4 | COLOR_GREEN, COLOR_DARK_RED << 4 | COLOR_DARK_MAGENTA, COLOR_YELLOW << 4 | COLOR_RED, 0,} },

	{	// Cushion magenta 
		172, GM_NO_CHANGE, 22,  // from: Cushion red  
		{ COLOR_RED << 4 | COLOR_MAGENTA, COLOR_DARK_RED << 4 | COLOR_DARK_GREEN, COLOR_YELLOW << 4 | COLOR_WHITE, 0,} },

	{	// Cushion yellow 
		173, GM_NO_CHANGE, 22,  // from: Cushion red  
		{ COLOR_YELLOW << 4 | COLOR_MAGENTA, COLOR_RED << 4 | COLOR_YELLOW, COLOR_DARK_RED << 4 | COLOR_DARK_YELLOW, 0,} },

	{	// Cushion white 
		174, GM_NO_CHANGE, 22,  // from: Cushion red  
		{ COLOR_RED << 4 | COLOR_WHITE, COLOR_DARK_RED << 4 | COLOR_BLACK, 0, 0,} },

	{	// Pouf light_cyan 
		180, GM_NO_CHANGE, 21,  // from: Pouf magenta  
		{ COLOR_MAGENTA << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_MAGENTA << 4 | COLOR_CYAN, 0, 0,} },

	{	// Pouf green 
		181, GM_NO_CHANGE, 21,  // from: Pouf magenta  
		{ COLOR_MAGENTA << 4 | COLOR_GREEN, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_GREEN, 0, 0,} },

	{	// Pouf yellow 
		182, GM_NO_CHANGE, 21,  // from: Pouf magenta  
		{ COLOR_YELLOW << 4 | COLOR_RED, COLOR_MAGENTA << 4 | COLOR_YELLOW, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_YELLOW, 0,} },

	{	// Pouf red 
		183, GM_NO_CHANGE, 21,  // from: Pouf magenta  
		{ COLOR_MAGENTA << 4 | COLOR_RED, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_RED, 0, 0,} },

	{	// Pouf white 
		184, GM_NO_CHANGE, 21,  // from: Pouf magenta  
		{ COLOR_MAGENTA << 4 | COLOR_WHITE, COLOR_DARK_MAGENTA << 4 | COLOR_GREY, 0, 0,} },

	{	// Jar magenta 
		185, GM_NO_CHANGE, 24,  // from: Jar green  
		{ COLOR_GREEN << 4 | COLOR_MAGENTA, COLOR_DARK_GREEN << 4 | COLOR_DARK_MAGENTA, 0, 0,} },

	{	// Jar red 
		186, GM_NO_CHANGE, 24,  // from: Jar green  
		{ COLOR_GREEN << 4 | COLOR_RED, COLOR_DARK_GREEN << 4 | COLOR_DARK_RED, 0, 0,} },

	{	// Jar yellow 
		187, GM_NO_CHANGE, 24,  // from: Jar green  
		{ COLOR_GREEN << 4 | COLOR_YELLOW, COLOR_DARK_GREEN << 4 | COLOR_DARK_YELLOW, 0, 0,} },

	{	// Jar light_cyan 
		188, GM_NO_CHANGE, 24,  // from: Jar green  
		{ COLOR_GREEN << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_GREEN << 4 | COLOR_CYAN, 0, 0,} },

	{	// Table white red 
		190, GM_NO_CHANGE, 23,  // from: Table yellow green 
		{ COLOR_YELLOW << 4 | COLOR_WHITE, COLOR_DARK_YELLOW << 4 | COLOR_GREY, COLOR_GREEN << 4 | COLOR_RED, 0,} },

	{	// B light_cyan magenta 
		191, GM_NO_CHANGE, 47,  // from: B white green 
		{ COLOR_WHITE << 4 | COLOR_LIGHT_CYAN, COLOR_GREY << 4 | COLOR_CYAN, COLOR_GREEN << 4 | COLOR_MAGENTA, COLOR_DARK_GREEN << 4 | COLOR_DARK_MAGENTA,} },

	{	// A light_cyan magenta 
		192, GM_NO_CHANGE, 48,  // from: A white green 
		{ COLOR_WHITE << 4 | COLOR_LIGHT_CYAN, COLOR_GREY << 4 | COLOR_CYAN, COLOR_GREEN << 4 | COLOR_MAGENTA, COLOR_DARK_GREEN << 4 | COLOR_DARK_MAGENTA,} },

	{	// L light_cyan magenta 
		193, GM_NO_CHANGE, 49,  // from: L white green 
		{ COLOR_WHITE << 4 | COLOR_LIGHT_CYAN, COLOR_GREY << 4 | COLOR_CYAN, COLOR_GREEN << 4 | COLOR_MAGENTA, COLOR_DARK_GREEN << 4 | COLOR_DARK_MAGENTA,} },

	{	// I light_cyan magenta 
		194, GM_NO_CHANGE, 50,  // from: I white green 
		{ COLOR_WHITE << 4 | COLOR_LIGHT_CYAN, COLOR_GREY << 4 | COLOR_CYAN, COLOR_GREEN << 4 | COLOR_MAGENTA, COLOR_DARK_GREEN << 4 | COLOR_DARK_MAGENTA,} },

	{	// Stair step green 
		195, GM_NO_CHANGE, 14,  // from: Stair step magenta  
		{ COLOR_DARK_MAGENTA << 4 | COLOR_DARK_GREEN, COLOR_MAGENTA << 4 | COLOR_GREEN, 0, 0,} },

	{	// Stair step light_cyan 
		196, GM_NO_CHANGE, 14,  // from: Stair step magenta  
		{ COLOR_DARK_MAGENTA << 4 | COLOR_CYAN, COLOR_MAGENTA << 4 | COLOR_LIGHT_CYAN, 0, 0,} },

	{	// Carpet green bottom left 
		200, GM_NO_CHANGE, 32,  // from: Carpet magenta bottom left 
		{ COLOR_MAGENTA << 4 | COLOR_GREEN, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_GREEN, 0, 0,} },

	{	// Carpet green top left 
		201, GM_NO_CHANGE, 33,  // from: Carpet magenta top left 
		{ COLOR_MAGENTA << 4 | COLOR_GREEN, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_GREEN, 0, 0,} },

	{	// Carpet green top right 
		202, GM_NO_CHANGE, 34,  // from: Carpet magenta top right 
		{ COLOR_MAGENTA << 4 | COLOR_GREEN, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_GREEN, 0, 0,} },

	{	// Carpet green bottom right 
		203, GM_NO_CHANGE, 35,  // from: Carpet magenta bottom right 
		{ COLOR_MAGENTA << 4 | COLOR_GREEN, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_GREEN, 0, 0,} },

	{	// Stones yellow 
		210, GM_NO_CHANGE, 15,  // from: Stones white  
		{ COLOR_WHITE << 4 | COLOR_YELLOW, COLOR_GREY << 4 | COLOR_DARK_YELLOW, 0, 0,} },

	{	// Stones magenta 
		211, GM_NO_CHANGE, 15,  // from: Stones white  
		{ COLOR_WHITE << 4 | COLOR_MAGENTA, COLOR_GREY << 4 | COLOR_DARK_MAGENTA, 0, 0,} },

	{	// Stones light_cyan 
		212, GM_NO_CHANGE, 15,  // from: Stones white  
		{ COLOR_WHITE << 4 | COLOR_LIGHT_CYAN, COLOR_GREY << 4 | COLOR_CYAN, 0, 0,} },

	{	// Stones with chains yellow 
		215, GM_NO_CHANGE, 16,  // from: Stones with chains white  
		{ COLOR_WHITE << 4 | COLOR_YELLOW, COLOR_GREY << 4 | COLOR_DARK_YELLOW, 0, 0,} },

	{	// Stones with chains light_cyan 
		216, GM_NO_CHANGE, 16,  // from: Stones with chains white  
		{ COLOR_WHITE << 4 | COLOR_LIGHT_CYAN, COLOR_GREY << 4 | COLOR_CYAN, 0, 0,} },

	{	// Gate magenta left bottom 
		220, GM_NO_CHANGE, 51,  // from: Gate green left bottom 
		{ COLOR_GREEN << 4 | COLOR_MAGENTA, COLOR_DARK_GREEN << 4 | COLOR_DARK_MAGENTA, 0, 0,} },

	{	// Gate magenta left top 
		221, GM_NO_CHANGE, 52,  // from: Gate green left top 
		{ COLOR_GREEN << 4 | COLOR_MAGENTA, COLOR_DARK_GREEN << 4 | COLOR_DARK_MAGENTA, 0, 0,} },

	{	// Gate magenta center top 
		222, GM_NO_CHANGE, 53,  // from: Gate green center top 
		{ COLOR_GREEN << 4 | COLOR_MAGENTA, COLOR_DARK_GREEN << 4 | COLOR_DARK_MAGENTA, 0, 0,} },

	{	// Gate magenta right top 
		223, GM_NO_CHANGE, 54,  // from: Gate green right top 
		{ COLOR_GREEN << 4 | COLOR_MAGENTA, COLOR_DARK_GREEN << 4 | COLOR_DARK_MAGENTA, 0, 0,} },

	{	// Gate magenta right bottom 
		224, GM_NO_CHANGE, 55,  // from: Gate green right bottom 
		{ COLOR_GREEN << 4 | COLOR_MAGENTA, COLOR_DARK_GREEN << 4 | COLOR_DARK_MAGENTA, 0, 0,} },

	{	// Gate door magenta 
		225, GM_NO_CHANGE, 56,  // from: Gate door green  
		{ COLOR_GREEN << 4 | COLOR_MAGENTA, COLOR_DARK_GREEN << 4 | COLOR_DARK_MAGENTA, 0, 0,} },

	{	// Crab a blue 
		226, GM_NO_CHANGE, 37,  // from: Crab a magenta 
		{ COLOR_MAGENTA << 4 | COLOR_BLUE, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_BLUE, 0, 0,} },

	{	// Crab b blue 
		227, GM_NO_CHANGE, 38,  // from: Crab b magenta 
		{ COLOR_MAGENTA << 4 | COLOR_BLUE, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_BLUE, 0, 0,} },

	{	// Sword bad guy left red 
		228, GM_NO_CHANGE, 41,  // from: Sword bad guy left green 
		{ COLOR_GREEN << 4 | COLOR_RED, COLOR_DARK_GREEN << 4 | COLOR_DARK_RED, 0, 0,} },

	{	// Sword bad guy right red 
		229, GM_NO_CHANGE, 42,  // from: Sword bad guy right green 
		{ COLOR_GREEN << 4 | COLOR_RED, COLOR_DARK_GREEN << 4 | COLOR_DARK_RED, 0, 0,} },

	{	// Sword bad guy left yellow 
		230, GM_NO_CHANGE, 41,  // from: Sword bad guy left green 
		{ COLOR_GREEN << 4 | COLOR_YELLOW, COLOR_DARK_GREEN << 4 | COLOR_DARK_YELLOW, 0, 0,} },

	{	// Sword bad guy right yellow 
		231, GM_NO_CHANGE, 42,  // from: Sword bad guy right green 
		{ COLOR_GREEN << 4 | COLOR_YELLOW, COLOR_DARK_GREEN << 4 | COLOR_DARK_YELLOW, 0, 0,} },

	{	// Sword bad guy left white 
		232, GM_NO_CHANGE, 41,  // from: Sword bad guy left green 
		{ COLOR_GREEN << 4 | COLOR_WHITE, COLOR_DARK_GREEN << 4 | COLOR_GREY, 0, 0,} },

	{	// Sword bad guy right white 
		233, GM_NO_CHANGE, 42,  // from: Sword bad guy right green 
		{ COLOR_GREEN << 4 | COLOR_WHITE, COLOR_DARK_GREEN << 4 | COLOR_GREY, 0, 0,} },

	{	// Sword bad guy left white 
		234, GM_NO_CHANGE, 41,  // from: Sword bad guy left green 
		{ COLOR_GREEN << 4 | COLOR_WHITE, COLOR_DARK_GREEN << 4 | COLOR_GREY, COLOR_DARK_BLUE << 4 | COLOR_GREY, COLOR_BLUE << 4 | COLOR_GREY,} },

	{	// Sword bad guy right white 
		235, GM_NO_CHANGE, 42,  // from: Sword bad guy right green 
		{ COLOR_GREEN << 4 | COLOR_WHITE, COLOR_DARK_GREEN << 4 | COLOR_GREY, COLOR_DARK_BLUE << 4 | COLOR_GREY, COLOR_BLUE << 4 | COLOR_GREY,} },

	{	// Crab a red 
		236, GM_NO_CHANGE, 37,  // from: Crab a magenta 
		{ COLOR_MAGENTA << 4 | COLOR_RED, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_RED, 0, 0,} },

	{	// Crab b red 
		237, GM_NO_CHANGE, 38,  // from: Crab b magenta 
		{ COLOR_MAGENTA << 4 | COLOR_RED, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_RED, 0, 0,} },

	{	// Crab a green 
		238, GM_NO_CHANGE, 37,  // from: Crab a magenta 
		{ COLOR_MAGENTA << 4 | COLOR_GREEN, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_GREEN, 0, 0,} },

	{	// Crab b green 
		239, GM_NO_CHANGE, 38,  // from: Crab b magenta 
		{ COLOR_MAGENTA << 4 | COLOR_GREEN, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_GREEN, 0, 0,} },

	{	// Crab a yellow 
		240, GM_NO_CHANGE, 37,  // from: Crab a magenta 
		{ COLOR_MAGENTA << 4 | COLOR_YELLOW, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_YELLOW, 0, 0,} },

	{	// Crab b yellow 
		241, GM_NO_CHANGE, 38,  // from: Crab b magenta 
		{ COLOR_MAGENTA << 4 | COLOR_YELLOW, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_YELLOW, 0, 0,} },

	{	// Bad guy left yellow 
		242, GM_NO_CHANGE, 39,  // from: Bad guy left yellow 
		{ COLOR_DARK_YELLOW << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_RED << 4 | COLOR_DARK_BLUE, 0, 0,} },

	{	// Bad guy right yellow 
		243, GM_NO_CHANGE, 40,  // from: Bad guy right yellow 
		{ COLOR_DARK_YELLOW << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_RED << 4 | COLOR_DARK_BLUE, 0, 0,} },

	{	// Sword bad guy left light_cyan 
		244, GM_NO_CHANGE, 41,  // from: Sword bad guy left green 
		{ COLOR_GREEN << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_GREEN << 4 | COLOR_CYAN, 0, 0,} },

	{	// Sword bad guy right light_cyan 
		245, GM_NO_CHANGE, 42,  // from: Sword bad guy right green 
		{ COLOR_GREEN << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_GREEN << 4 | COLOR_CYAN, 0, 0,} },

	{	// Spider a 
		246, GM_NO_CHANGE, 62,  // from: Spider  a  
		{ COLOR_BLUE << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_BLUE << 4 | COLOR_CYAN, 0, 0,} },

	{	// Spider b 
		247, GM_NO_CHANGE, 63,  // from: Spider  b  
		{ COLOR_BLUE << 4 | COLOR_LIGHT_CYAN, COLOR_DARK_BLUE << 4 | COLOR_CYAN, 0, 0,} },

	{	// Spider a 
		248, GM_NO_CHANGE, 62,  // from: Spider  a  
		{ COLOR_BLUE << 4 | COLOR_GREEN, COLOR_DARK_BLUE << 4 | COLOR_DARK_GREEN, 0, 0,} },

	{	// Spider b 
		249, GM_NO_CHANGE, 63,  // from: Spider  b  
		{ COLOR_BLUE << 4 | COLOR_GREEN, COLOR_DARK_BLUE << 4 | COLOR_DARK_GREEN, 0, 0,} },

	{	// Spider a 
		250, GM_NO_CHANGE, 62,  // from: Spider  a  
		{ COLOR_BLUE << 4 | COLOR_RED, COLOR_DARK_BLUE << 4 | COLOR_DARK_RED, 0, 0,} },

	{	// Spider b 
		251, GM_NO_CHANGE, 63,  // from: Spider  b  
		{ COLOR_BLUE << 4 | COLOR_RED, COLOR_DARK_BLUE << 4 | COLOR_DARK_RED, 0, 0,} },

	{	// Snake body 
		252, GM_NO_CHANGE, 64,  // from: Snake body  
		{ COLOR_GREEN << 4 | COLOR_MAGENTA, COLOR_DARK_GREEN << 4 | COLOR_DARK_MAGENTA, 0, 0,} },

	{	// Snake body 
		253, GM_NO_CHANGE, 64,  // from: Snake body  
		{ COLOR_GREEN << 4 | COLOR_YELLOW, COLOR_DARK_GREEN << 4 | COLOR_DARK_YELLOW, 0, 0,} },

	{	// Stair step yellow 
		254, GM_NO_CHANGE, 14,  // from: Stair step magenta  
		{ COLOR_MAGENTA << 4 | COLOR_YELLOW, COLOR_DARK_MAGENTA << 4 | COLOR_DARK_YELLOW, 0, 0,} },

};




#define N_GRAPHICS_MODIFIERS (sizeof(graphics_modifiers)/sizeof(graphics_modifiers[0]))


#endif // GRAPHICSMOD_DATA_H
