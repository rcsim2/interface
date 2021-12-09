//-----------------------------------------------------------------------------
// File: tabdlg.h
//
// Desc: tab control procedures for R/C Sim Transmitter Calibration
//
//
// Copyright (c) 2000 Black Sphere Corp.
//------------------------------------------------------------------------------
#pragma once
#ifndef __TABDLG_H
#define __TABDLG_H


BOOL CALLBACK Tab1DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Tab2DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Tab3DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Tab4DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Tab5DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



LRESULT APIENTRY ProgressBar1SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar2SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar3SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar4SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar5SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar6SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar7SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar8SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar9SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar10SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


LRESULT APIENTRY TrackBar1SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar2SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar3SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar4SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar5SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar6SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar7SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar8SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar9SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar10SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 



#endif // __TABDLG_H