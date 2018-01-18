# Assignment 1 : Text Rain (Spring Day Version)
Nikki Kyllonen | kyllo089 | CSCI4611 | 2/17/2017

## Overview
This Processing program immitates the original `Text Rain` program by Romy Achituv and Camille Utterback. However, in this version, the lyrics to a Korean song titled `봄날 (Spring Day)` is written to the video in place of the original poem.

## Controls
- `UP` and `DOWN` arrow keys : increase and decrease the `threshold` value
- `LEFT` and `RIGHT` arrow keys : decrease and increase the number of SBlock objects drawn per second `numDrawnPer`
- `SPACE` : enables and disables a black and white debugging mode

## Implementation
- `void loadLyrics()` :
	- parses txt file containing song lyrics
	- constructs a new SBlock object for every syllable block and stores these into the origBlocks and notDispBlocks ArrayLists
	- x positions are determined based off of total number of SBlocks and the width of the canvas by "folding" the origBlocks elements so that one tenth of the array spans the width of the canvas; variation is included by adding a randomly generated float
	- y positions are randomly determined; by including negative values, the intention was to better vertically spread out the SBlocks
- `SBlock` :
	- object class that holds all information needed to draw a syllable block onto the canvas and move it down like rain
	- `SBlock::move()` : checks for dark edges and determines the new position of a specific SBlock
	- `SBlock::edgeFollow()` : called by `move()` when a dark edge is detected; intended to make SBlocks slide down sloped edges
- `void grabBlocksToDisplay()` : called by `draw()` in order to choose which SBlocks to draw to the canvas; populates and depopulates the `dispBlocks` and `notDispBlocks` arrays respectively
- `void draw()` :
	- grayscales the current video frame
	- determines when to draw new characters to the canvas by using the `millis()` function to determine, using the current `numDrawnPer` value
	- calls the `draw()` function for all the SBlocks currently falling
	- displays current `numDrawnPer` and `threshold` values to the canvas

## Included Files
- `TextRainStudent` : Processing Source Code
- `/data/NanumGothic` : TrueType font (TTF) file that includes Hangul characters
- `/data/spring_day_hangul_spaced` : text file containing song lyrics

## More Information
More information on the song and the lyrics (ROM|ENG|HAN) can be found at this site:
`https://colorcodedlyrics.com/2017/02/bts-bangtansonyeondan-spring-day-bomnal`.