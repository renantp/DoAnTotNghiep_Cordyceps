//=========Chứa mã HTLM Website==//
#ifndef _HTML_H_
#define _HTML_H_
#include <Arduino.h>

const char TestPage[] PROGMEM = R"=====(
<!DOCTYPE html> 
  <html>
   <head>
   </head>
<body>
<h1>My logo!!!</h1>
<svg xmlns="http://www.w3.org/2000/svg" width="80px" height="83px">
  <g fill="#2f2d3b">
  <path d="M651 735.5 c0 -2.5 0.4 -3.5 1.6 -3.5 0.8 0 1.3 0.4 0.9 0.9 -0.3 0.5 -0.1 1.2 0.4 1.5 1.3 0.9 0.1 4.6 -1.5 4.6 -0.9 0 -1.4 -1.2 -1.4 -3.5z"/>
  <path d="M660 731.6 c0 -0.8 -0.8 -1.7 -1.7 -1.9 -1.3 -0.3 -1.1 -0.5 0.6 -0.6 2 -0.1 2.1 -0.3 1.1 -1.6 -1 -1.3 -0.9 -1.5 0.9 -1.5 1.7 0 2.1 0.6 2.1 3.5 0 2.4 -0.5 3.5 -1.5 3.5 -0.8 0 -1.5 -0.6 -1.5 -1.4z"/>
  <path d="M654 728 c0 -1.1 0.4 -2 0.9 -2 0.5 0 0.7 0.9 0.4 2 -0.3 1.1 -0.7 2 -0.9 2 -0.2 0 -0.4 -0.9 -0.4 -2z"/>
  <path d="M612 530.6 c-16.7 -4.4 -29.2 -8.5 -32.9 -10.7 -4 -2.4 -5.4 -7.6 -4.1 -15.3 2.5 -15.7 9.3 -21.5 38.6 -33.2 11.8 -4.8 12 -4.8 23.4 -4.9 13.8 0 16.5 1 19.8 7.5 4.6 9.3 2.2 25.6 -6.7 44.4 -4.9 10.4 -5.1 10.7 -10.3 13.1 -7.1 3.3 -12.4 3.1 -27.8 -0.9z"/>
  <path d="M216.1 403.8 c-2.3 -2.9 -4.6 -5.8 -5.1 -6.5 -0.7 -0.9 -0.1 -1.3 2.2 -1.5 1.8 -0.2 3.7 -1.2 4.5 -2.5 1.4 -2.1 1.2 -2.5 -3.2 -6.5 -4.6 -4.2 -8.5 -10.2 -8.5 -13.3 0 -0.8 -0.4 -1.5 -0.8 -1.5 -0.4 0 -0.6 -1.6 -0.4 -3.5 0.2 -1.9 0.6 -3.5 0.8 -3.5 0.2 0 2.3 1 4.8 2.1 4.5 2.2 7.2 2.1 8.2 -0.5 0.3 -0.8 -1.3 -4.6 -3.5 -8.5 -2.2 -3.9 -4.1 -7.5 -4.1 -7.9 -0.2 -2.3 -2.4 -8.2 -3 -8.2 -0.4 0 -0.7 -0.6 -0.7 -1.2 0.2 -2.1 -1.5 -8.8 -2.2 -8.8 -0.3 0 -0.7 -1 -0.7 -2.2 -0.3 -4.6 -2.8 -18.8 -3.4 -18.8 -0.3 0 -0.4 -1.8 -0.2 -4 0.2 -2.2 0 -4 -0.5 -4 -0.5 0 -0.7 -1.6 -0.5 -3.5 0.2 -1.9 0.1 -4.2 -0.2 -5.1 -2.3 -6 -1.9 -110.3 0.5 -123.1 0.6 -3.4 0.8 -6.6 0.5 -7.1 -0.3 -0.6 -0.1 -1.2 0.5 -1.4 1.4 -0.5 4.2 -19.3 3.5 -23.5 -0.3 -2 0 -3.3 0.6 -3.3 0.6 0 0.9 -1.6 0.6 -4 -0.2 -2.2 -0.1 -4 0.2 -4 0.9 0 4.2 -32.9 3.5 -34.1 -0.4 -0.5 -0.1 -0.9 0.6 -0.9 0.7 0 1 -1.1 0.7 -3 -0.3 -1.6 0 -3 0.5 -3 0.5 0 0.7 -2 0.5 -4.5 -0.2 -2.5 0 -4.5 0.4 -4.5 0.5 0 0.8 -1.1 0.8 -2.5 0 -3.4 1.1 -3.1 5.1 1.3 3.8 4 28.9 40.8 28.9 42.2 0 0.4 0.4 1 0.8 1.2 1 0.4 16.2 22.8 16.2 23.8 0 0.5 0.4 1 0.8 1.2 0.5 0.2 4.2 5.3 8.3 11.3 4.1 6.1 7.6 11.2 7.9 11.5 0.3 0.3 3.4 4.3 7 9 8.8 11.7 12.4 16.4 13.1 17 0.3 0.3 1.8 2.3 3.5 4.5 1.6 2.2 3.4 4.5 3.9 5.1 0.6 0.6 1.7 2.2 2.5 3.4 0.8 1.3 2.5 3.5 3.7 5 2.3 2.8 9.3 14.7 9.8 16.5 0.1 0.6 1.1 3 2.3 5.5 1.1 2.5 2.2 5.4 2.4 6.5 0.1 1.1 0.6 2.2 0.9 2.5 0.3 0.3 1.8 5.2 3.4 11 1.5 5.8 3.5 12.1 4.6 14.1 1 2 1.5 3.9 1.2 4.3 -0.4 0.3 -0.1 0.6 0.5 0.6 0.6 0 1.2 1 1.4 2.3 0.2 1.5 1.1 2.3 2.9 2.5 2.1 0.2 3.5 -0.7 6.8 -4.4 2.3 -2.6 4.3 -4.5 4.5 -4.3 1.4 1.7 1.4 5.8 -0.1 7 -0.9 0.8 -2.9 3.4 -4.4 5.8 -2.2 3.4 -2.6 4.8 -1.8 6.2 1.5 2.9 5.1 2.2 9.3 -1.8 4.6 -4.5 5.6 -3.6 5.5 5.2 l0 6 -5.6 3.3 c-3.1 1.7 -6.4 3.2 -7.5 3.2 -1.1 0 -1.7 0.4 -1.4 0.9 0.3 0.5 -2.6 2.8 -6.4 5.1 -7.6 4.4 -36.2 23.4 -44.5 29.4 -2.7 2 -5.8 3.6 -6.7 3.6 -1 0 -1.8 0.6 -1.8 1.3 0 1.6 -13.4 11.7 -15.5 11.7 -0.9 0 -1.4 0.4 -1 0.9 0.3 0.5 0 1.3 -0.6 1.7 -1.5 0.9 -5.3 -3.5 -5.6 -6.4 -0.3 -2.2 0 -2.3 6 -2 6 0.3 6.2 0.2 6.3 -2.2 0.1 -1.8 -1.7 -4.4 -6.2 -9 -7 -7.4 -10.2 -13 -10.5 -18.7 -0.4 -5.7 0.6 -6.7 4 -4.2 3 2.2 7.8 2.3 7.4 0.1 -0.2 -0.6 0.4 -1.2 1.2 -1.2 2.8 0 2.6 -2.6 -0.4 -7.6 -1.7 -2.7 -4.3 -8.3 -5.9 -12.4 -1.6 -4.1 -3.3 -8.4 -3.7 -9.5 -0.5 -1.1 -0.7 -2.6 -0.6 -3.2 0.1 -0.7 -0.2 -1.3 -0.6 -1.3 -0.4 0 -0.9 -1.4 -1 -3.1 -0.1 -1.7 -0.7 -3.7 -1.3 -4.4 -0.5 -0.6 -0.7 -1.5 -0.4 -1.8 0.4 -0.3 0.3 -1.4 -0.1 -2.4 -0.4 -1 -0.7 -2.8 -0.6 -4 0.1 -1.3 -0.2 -2.3 -0.7 -2.3 -0.5 0 -0.7 -1.1 -0.4 -2.5 0.2 -1.5 0 -2.5 -0.7 -2.5 -0.7 0 -0.9 -0.4 -0.6 -1 0.5 -0.8 -1.9 -16.3 -7.3 -47.2 -0.2 -1.6 -0.9 -2.8 -1.4 -2.8 -0.5 0 -0.7 -0.4 -0.3 -0.9 0.3 -0.6 -0.3 -4.4 -1.4 -8.6 -1.1 -4.2 -2.6 -12.3 -3.2 -18.1 -0.7 -5.7 -1.6 -10.4 -2 -10.4 -0.5 0 -0.6 -0.7 -0.3 -1.5 0.7 -1.7 -2.4 -15.3 -3.6 -16 -0.5 -0.3 -0.6 -1.4 -0.3 -2.5 0.3 -1.1 0.1 -2 -0.5 -2 -0.6 0 -0.8 -0.9 -0.5 -2 0.3 -1.2 0 -2 -0.6 -2 -0.7 0 -1 -0.4 -0.7 -0.9 0.8 -1.2 -1.5 -11.1 -2.5 -11.1 -0.5 0 -0.6 -0.7 -0.3 -1.5 0.4 -0.8 0.1 -1.5 -0.7 -1.5 -0.7 0 -0.9 -0.3 -0.6 -0.6 0.4 -0.4 -0.5 -2.9 -2 -5.6 -2.7 -5 -5.4 -6.5 -9.3 -5.3 -1.7 0.5 -2 1.2 -1.5 3.2 0.3 1.4 1.2 3.2 2 3.9 1.6 1.6 4.3 10.5 4.7 15.4 0.2 1.9 0.6 3.7 0.9 4 1.1 0.9 5 18.9 5.4 24.8 0.2 3.1 0.7 6.5 1 7.5 0.4 0.9 0.5 2.9 0.3 4.2 -0.3 1.4 -0.2 2.5 0.2 2.5 0.6 0 1.7 6.6 2.3 14 0.1 1.4 0.5 2.7 0.8 3 1 0.8 5 22.6 5.2 28.1 0.1 2.7 0.6 4.9 1.1 4.9 0.4 0 0.6 1.1 0.4 2.5 -0.3 1.4 -0.1 2.5 0.4 2.5 0.5 0 0.8 1 0.7 2.3 -0.1 1.2 0.1 2.9 0.4 3.7 1.2 3.2 2.8 13 2.2 14 -0.3 0.5 -0.1 1 0.6 1 0.7 0 1 1.1 0.7 3 -0.3 1.7 -0.1 3 0.4 3 0.4 0 0.8 1.2 0.8 2.8 0 1.5 0.7 5.6 1.6 9.2 0.8 3.6 1.7 7.4 1.8 8.5 0.2 1.1 0.6 2.7 1 3.5 2.2 5.5 3.5 10.4 2.9 10.9 -0.5 0.3 -0.8 1.1 -0.9 1.6 0 0.6 -1.1 3.2 -2.3 6 -3.6 7.5 -3.3 16.9 0.8 24.6 3.4 6.6 3.8 10.6 1.7 16.7 -0.8 2.3 -1.1 4.4 -0.7 4.7 0.5 0.2 1.1 1.8 1.4 3.4 0.4 1.6 2.1 4.6 3.9 6.6 l3.2 3.7 -5.4 4.9 c-3 2.7 -6.3 4.9 -7.3 4.9 -0.9 0 -1.6 0.3 -1.5 0.8 0.1 0.4 -1.3 2.4 -3.2 4.6 -1.9 2.1 -5.5 6 -7.9 8.7 -2.4 2.7 -5 4.9 -5.8 4.9 -0.8 0 -1.1 0.5 -0.7 1.1 0.7 1.2 -7.4 9.9 -9.3 9.9 -0.7 0 -1.2 0.5 -1 1.1 0.1 0.6 -0.3 1.3 -1 1.6 -0.6 0.2 -3 -2 -5.2 -4.9z"/>
  <path d="M618 211.3 c-2.4 -4.3 -4.7 -8 -5 -8.3 -0.4 -0.3 -1 -1.5 -1.4 -2.8 -1.1 -3.3 -5.5 -8.2 -7.6 -8.2 -1 0 -3 0.7 -4.6 1.5 -2.8 1.4 -5.4 0.6 -5.4 -1.7 0 -1.2 -4.5 -7.2 -9.4 -12.5 -3.8 -4.1 -6.6 -5.2 -9.3 -3.5 -1.6 1 -3.4 6 -2.4 6.6 1.2 0.8 0.8 5.1 -0.5 5.1 -1.8 0 -19.4 -17.3 -19.4 -19 0 -0.8 -0.4 -1.5 -0.9 -1.5 -0.6 0 -0.7 -1.6 -0.4 -3.5 0.4 -2.5 0.2 -3.5 -0.7 -3.5 -0.7 0 -1 -0.3 -0.7 -0.6 1 -1 -3.3 -4.4 -5.4 -4.4 -1 0 -3.3 1.3 -5.1 2.9 -3.1 2.6 -6.7 3.3 -12 2.2 -1.7 -0.3 0.6 -10.9 3.3 -15.4 1.6 -2.7 2.7 -5.3 2.3 -5.8 -0.3 -0.5 0.1 -0.9 0.8 -0.9 0.7 0 2.8 -2 4.5 -4.4 1.8 -2.3 6 -7.5 9.3 -11.5 3.3 -3.9 5.8 -7.6 5.5 -8.1 -0.3 -0.6 0 -1 0.9 -1 1.7 0 11.7 -11 11 -12.1 -0.2 -0.4 0.6 -1.3 1.8 -2 3.3 -1.6 32.8 -31.1 32.2 -32.1 -0.3 -0.4 0.1 -0.8 1 -0.8 2 0 14.8 -14.2 14.1 -15.7 -0.2 -0.7 0.2 -1.3 0.9 -1.3 0.8 0 2.8 -2.3 4.6 -5 1.8 -2.8 3.7 -4.9 4.3 -4.7 1.4 0.4 6.8 16.2 7 20.4 0.1 1.8 0.5 3.3 0.8 3.3 0.4 0 1.3 3.9 2 8.7 1.7 10.8 1.9 56.5 0.3 57 -0.6 0.2 -0.8 1.7 -0.6 3.3 0.2 1.7 0 3 -0.4 3 -0.5 0 -1 2.1 -1.1 4.8 -0.1 2.6 -1.6 8.9 -3.2 14.1 -1.7 5.2 -3.1 10.2 -3.1 11.1 0 2.4 2.1 4 5.3 4 2.7 0 2.8 0.2 2.2 3.3 -1.5 7.3 -5.9 20.7 -9.2 27.7 -4.1 8.8 -4.3 11.3 -1.3 14.5 2 2.1 2.1 4.5 0.1 4.5 -0.4 0 -2.7 -3.5 -5.1 -7.7z"/>
 </g>
 <g fill="#c12f27">
  <path d="M200 764 c-0.7 -1.7 -2.5 -4 -4.1 -5.1 -3.8 -2.7 -4.3 -4.6 -4.3 -17.4 -0.1 -9.7 -0.4 -11.4 -2.4 -14.8 -2.8 -4.3 -6.8 -6.7 -11.7 -6.9 -5 -0.2 -7.5 2.4 -7.5 7.8 0 7.6 -0.5 7.9 -7.2 3.2 -7.8 -5.5 -20.3 -18.7 -26.8 -28.5 -6.6 -9.8 -15.9 -29.1 -19.8 -40.8 -5.8 -17.6 -7.7 -28.5 -7.7 -45.5 0 -18 1.7 -27.4 7.3 -40.9 6.7 -16.3 13.4 -26 35.3 -51.6 7.5 -8.9 15.4 -18.3 17.5 -21 2 -2.8 5.9 -7.8 8.5 -11.2 2.6 -3.4 4.9 -6.7 5.1 -7.4 0.3 -0.8 2.2 0.1 5.5 2.5 6 4.3 9 4.7 9.9 1.1 0.7 -2.8 2.1 -2.9 3.8 -0.2 0.8 1.2 0.3 2.4 -2.2 5.4 -19.4 22.7 -31.9 45.1 -37.9 68.3 -3.1 11.8 -4.3 35.7 -2.3 47.8 5.2 32.7 28.7 70.4 55.1 88.3 3.4 2.3 10.5 6.1 15.8 8.6 15.1 6.9 16.1 7.5 21.4 12.3 4.6 4.1 14.2 15.9 13.5 16.6 -0.2 0.1 -2.8 -0.4 -5.8 -1.2 -7.5 -1.9 -21.2 -1.8 -25.1 0.2 -4 2 -6.3 7.3 -4.9 11 0.6 1.5 3.2 5.1 5.9 8 4.5 4.9 7 9.9 5.7 11.2 -0.3 0.3 -5 -3.8 -10.5 -9.1 -12.4 -12 -18 -15.7 -23.8 -15.7 -3.3 0 -5 0.6 -6.8 2.5 -1.4 1.3 -2.5 3.1 -2.5 4 0 0.9 1.6 5.4 3.5 10 3.1 7.6 3.6 11.5 1.5 11.5 -0.4 0 -1.3 -1.4 -2 -3z m-28.7 -52.1 c3.2 -1.7 3.7 -2.8 3.7 -8.4 0 -9.2 -9.5 -20.5 -17.3 -20.5 -3.3 0 -4.7 1.8 -4.7 6.2 0 2.1 -0.7 5.5 -1.5 7.5 -3.2 7.7 -2 12.3 4 14.9 3.7 1.6 12.9 1.8 15.8 0.3z"/>
  <path d="M58.8 536.8 c-0.1 -2.4 -0.2 -5.4 -0.3 -6.8 -0.6 -14.5 14.4 -40.8 30.8 -53.8 12.8 -10.2 24.7 -15.1 42.6 -17.6 14.9 -2.1 21.1 -4.5 30.8 -11.7 2.9 -2.2 5.7 -3.9 6.4 -3.9 1.1 0 0.7 23.8 -0.7 35.6 l-0.6 5.2 -15.6 7.2 c-8.7 4 -18.4 8.4 -21.7 9.7 -13.9 5.6 -16.9 7.6 -27.7 18.5 -5.9 6 -12.6 13.3 -14.8 16.3 -2.3 3.1 -4.5 5.3 -5.1 5 -0.5 -0.4 -0.9 -3.2 -0.9 -6.4 0 -6.2 -1.1 -8.1 -4.9 -8.1 -1.8 0 -4.3 2.1 -9.4 7.5 -3.8 4.1 -7.3 7.5 -7.8 7.5 -0.5 0 -1 -1.9 -1.1 -4.2z"/>
  <path d="M211.2 483.3 c-4.3 -5.5 -10.2 -15.8 -10.2 -18 0 -1 1 -3.4 2.2 -5.3 l2.1 -3.5 1.9 4.5 c2.6 6 5.4 8.8 9.5 9.6 3 0.6 3.4 1 2.8 2.8 -0.4 1.1 -1.2 4.7 -1.7 7.9 -0.7 3.4 -1.6 5.7 -2.4 5.7 -0.7 0 -2.6 -1.7 -4.2 -3.7z"/>
  <path d="M227.4 451.5 c5.1 -14.7 28.1 -39.7 53 -57.8 16 -11.7 22.9 -17.1 24 -18.9 1.5 -2.5 -1.4 -5.3 -4.6 -4.5 -6.7 1.8 -35 21.3 -51.9 35.9 -9 7.7 -23.3 24.1 -28.5 32.5 -2.2 3.7 -4.5 6.8 -5.1 7 -1.4 0.5 -2.3 -4.6 -1.3 -7.3 1.4 -3.5 13.1 -17.7 28 -33.9 24.8 -26.9 45.1 -43.7 84.5 -70.1 32.5 -21.8 54.1 -34.1 77.9 -44.4 52.8 -22.9 95.7 -35 179.6 -50.5 24.7 -4.6 38 -2.8 48.6 6.8 2.2 1.9 5.9 4.5 8.4 5.8 3.8 2 4.1 2.4 2.3 2.7 -1.2 0.2 -3 -0.1 -4 -0.7 -3.6 -1.9 -18.6 -4.1 -27.8 -4.1 -13.8 0 -23.8 2.2 -47.7 10.3 -23.9 8.3 -28 10.2 -27.6 13.1 0.8 5.4 8.8 4.5 33.8 -3.8 21.2 -7.1 29.1 -8.7 42 -8.9 9 -0.1 12.1 0.4 22 3.2 15.8 4.4 16.5 4.8 19.4 9.2 1.4 2.1 2.6 4 2.6 4.2 0 0.2 -2.8 -0.2 -6.2 -0.8 -3.5 -0.7 -14.7 -1.8 -24.9 -2.4 -19.5 -1.3 -20 -1.3 -33.4 2.8 -3.3 1.1 -9.6 2.9 -14 4.1 -22.3 6.1 -49.2 14.6 -90 28.1 -79.4 26.5 -94 32.4 -130.5 52.7 -54.8 30.5 -93.7 57.6 -121.9 84.6 -4.1 3.9 -7.1 6.2 -6.7 5.1z m293.3 -167 c3.9 -3.9 3.9 -5 0.1 -7.8 -3.1 -2.3 -7.4 -1.8 -12.5 1.3 -2.6 1.5 -3.3 2.7 -3.3 5 0 3.5 2.2 4.8 8.4 4.9 3.1 0.1 4.5 -0.6 7.3 -3.4z"/>
 </g>
 <g fill="#75b844">
  <path d="M439.7 704.3 c-0.5 -0.7 -2.3 -1.3 -4 -1.3 -1.8 -0.1 -7.2 -0.9 -12.2 -2 -14.2 -3 -46.8 -8.7 -60 -10.5 -19.2 -2.6 -50.6 -10.2 -53.4 -13 -1 -1 -1.1 -2 -0.3 -4.6 1.3 -4 0.3 -9.8 -1.9 -11.2 -1.1 -0.7 -3.5 -0.2 -8.5 1.8 -13.9 5.6 -23 6.7 -36 4.5 -5.1 -0.9 -14.1 -3.6 -15.3 -4.6 -0.2 -0.2 0.8 -1.3 2.3 -2.5 3.7 -2.9 9.8 -12.7 9.4 -15.2 -0.4 -3.1 -4.3 -4.8 -8 -3.5 -4.4 1.5 -14.6 0.4 -20.3 -2.4 -8.3 -3.9 -8.2 -3.5 -2.7 -8.8 9 -8.6 9.7 -11.9 7.7 -36.6 -0.4 -5.6 -0.5 -11.3 -0.1 -12.8 0.5 -2.2 1.2 -2.6 4.5 -2.6 3.3 0 4.8 -0.8 9.4 -5 3 -2.7 5.8 -5 6.1 -5 0.3 0 0.6 2.9 0.6 6.5 0 18 14.4 38.5 27.1 38.5 3.4 0 8.6 -4.6 12.4 -11.1 2.7 -4.6 5.4 -4.4 8.1 0.4 2.6 4.8 11.8 14.4 15.6 16.3 4.6 2.4 10.5 1.8 13.5 -1.3 2.4 -2.6 13.8 -20.5 17 -26.6 1.8 -3.5 1 -6.1 -2.3 -7.6 -2.2 -0.9 -3.1 -0.9 -5 0.4 -2.6 1.7 -12.6 13.9 -16.3 19.9 -1.3 2 -2.8 3.5 -3.5 3.3 -1.3 -0.4 -8.1 -9 -10.2 -13.1 -0.9 -1.5 -1.6 -4.8 -1.6 -7.1 0 -5.8 -2.5 -8.5 -8 -8.5 -3.5 0 -4.7 0.6 -7.8 3.9 -2 2.1 -5.9 7.3 -8.5 11.5 -2.7 4.2 -5.4 7.6 -6 7.6 -1.6 0 -7.1 -7.9 -10.2 -14.7 -2.8 -6 -2.9 -7.2 -2 -25.8 0.6 -12.4 0 -15 -3.8 -15.9 -2.2 -0.6 -3.6 0 -7.3 3.1 -2.6 2 -6.6 5.7 -8.9 8.1 -2.2 2.5 -4.6 4.2 -5.1 3.8 -1.7 -1.1 -0.2 -14.6 3 -26.3 3.5 -12.9 3.3 -15.6 -1.3 -17.2 -3.4 -1.2 -5.3 -0.4 -14.7 6.2 -2.2 1.6 -6 3.1 -8.3 3.4 -5 0.7 -5 1.1 0.6 -11.5 3.8 -8.5 6.4 -20.1 6.5 -28.5 0 -5 4.7 -19.8 7.9 -25 3.8 -6.1 24.8 -25.7 39.6 -36.8 17.4 -13.2 71.8 -47 89.4 -55.6 l7.5 -3.7 20.3 0.3 c11.2 0.1 21.7 0.6 23.4 1.1 1.6 0.5 5.9 2.5 9.4 4.4 7.5 4.2 11.2 4.5 14.8 1.2 4.1 -3.8 4.5 -7.2 1.1 -10.7 -3.4 -3.5 -14.6 -10.3 -22.3 -13.4 -3.5 -1.5 -5 -2.6 -4.1 -2.9 0.8 -0.3 8 -3 16 -5.9 21.9 -8.2 71.1 -24.6 108.5 -36.2 6.6 -2 15.3 -4.8 19.4 -6.2 4 -1.3 7.5 -2.2 7.8 -1.9 1 0.9 -8.3 16.9 -13.4 23.2 -2.9 3.5 -5.3 7.1 -5.3 8 0 0.8 1.4 2.9 3 4.6 2.8 2.7 3.6 3 7.5 2.5 6.2 -0.6 8.9 -2.7 14.2 -11 5.9 -9 10.2 -19.3 11 -25.9 0.4 -3 1.5 -6 2.8 -7.6 l2.1 -2.5 24.5 -0.1 c20.1 -0.1 25.6 0.2 31.2 1.7 12.8 3.2 18.7 8.8 22.3 20.7 1.9 6.3 6.8 10.7 18.5 16.3 5.2 2.5 10.5 4.9 11.8 5.4 2.4 0.9 2.3 1 -2.2 3.9 -5.2 3.2 -5.9 5.7 -2.9 9.4 1.3 1.6 3.5 2.4 9.7 3.2 4.4 0.5 8.4 1.4 8.9 1.8 0.5 0.5 -0.1 2.7 -1.2 5.2 -2.1 4.1 -6.9 7.8 -16.6 12.8 -6 3.1 -6.8 8.3 -1.8 11.6 1.5 1 5.4 2 8.7 2.3 4.6 0.5 6.1 1 6.3 2.3 0.3 1.3 -1.2 2.4 -5.5 4.3 -12.8 5.7 -13.4 10 -3.2 23.1 4.9 6.4 6.8 11.2 4.8 12.5 -0.8 0.5 -2.4 0.9 -3.6 0.9 -2.7 0 -2.7 0.3 -4.2 16 -1.8 18.7 -7.2 39.1 -7.8 29.3 0 -1.4 -2.2 -6.9 -4.9 -12.2 -3.8 -7.8 -6.2 -11.1 -12.8 -17.7 -11.7 -11.7 -28.5 -22.2 -46.1 -28.9 -5.5 -2.1 -10.2 -4 -10.4 -4.1 -0.2 -0.2 2.1 -2.6 5.1 -5.3 6.5 -5.9 11.2 -14.3 12.9 -22.9 1.5 -7.9 0.6 -24.3 -2 -36.7 -2.2 -10.6 -8.5 -25.9 -11.5 -28 -2.2 -1.6 -6.4 -1.9 -8.2 -0.7 -1.6 1 -18.1 29.3 -21.5 36.7 -1.5 3.3 -3.5 8.7 -4.4 11.9 -1 3.5 -3 7.3 -5.1 9.6 -1.9 2.1 -3.4 4.2 -3.4 4.9 0 0.6 1.7 1.9 3.8 3 4.8 2.4 5.3 3.1 9 11.3 l3.1 6.8 -2.8 0 c-1.6 0 -6.9 -2.5 -12.7 -6 -5.4 -3.3 -10.2 -6 -10.6 -6 -0.4 0 -0.8 1.2 -1 2.8 -0.3 2.4 -0.8 2.7 -4.5 3 -4.1 0.3 -4.3 0.2 -7.7 -5.2 -5 -7.8 -15.9 -19.5 -22.3 -23.8 -3 -2.1 -5.8 -3.8 -6.2 -3.8 -0.3 0 -0.5 10.7 -0.3 23.7 0.3 27.9 0.1 28.6 -6.9 29.1 -3.9 0.3 -4.7 0 -5.6 -2 -0.8 -1.9 -1.1 -1.4 -2.2 4.2 -0.7 3.6 -2.8 12.2 -4.7 19.2 -3.2 11.6 -3.5 13.5 -2.9 20.7 0.7 7.7 0.7 8 -2.4 12.4 -4.2 6.1 -10.5 11.5 -24.5 20.9 -27.2 18.2 -47.1 24.8 -75 24.8 -13.9 0 -17.6 0.5 -17.6 2.4 0.1 0.6 3 4.3 6.5 8.1 5.6 6 6.5 7.5 6.5 10.8 l0 3.7 -11 0 -11.1 0 4.3 6.2 c2.4 3.3 7.9 10 12.3 14.7 11.1 11.8 12.5 13.8 12.5 17.7 l0 3.4 -7.5 0 -7.5 0 1.5 2.4 c2.4 3.6 12.9 11.5 20.2 15.2 3.6 1.8 10.4 4.4 15.1 5.9 13.6 4.1 22.5 7.4 24.7 9.1 2 1.6 2 1.6 -0.2 3.1 -3.1 2.1 -2.9 5.8 0.2 7.9 2.2 1.4 3 1.5 5 0.4 1.4 -0.7 2.7 -1.1 2.9 -0.9 1.9 2.2 9.6 20.3 9.6 22.6 0 4.9 -1.9 5.7 -10.8 4.8 -7.4 -0.8 -24.3 -0.7 -25.1 0 -0.2 0.2 1.2 2.6 3.1 5.2 2.2 3.1 7.5 7.7 15.1 13.1 11.4 8.2 11.7 8.5 11.7 12.1 l0 3.8 -8.2 0.6 c-4.6 0.4 -10.2 0.7 -12.5 0.7 -2.3 0 -4.4 0.4 -4.7 0.9 -0.4 0.5 1 4 2.9 7.7 2.3 4.5 3.5 7.9 3.3 9.9 -0.2 1.9 -0.6 2.6 -1.1 1.8z m-58.1 -94 l3.4 -3.8 -2.5 -2.2 c-3.2 -2.8 -3.2 -2.8 -7.2 -0.3 -3.6 2.2 -5.2 6.2 -3.3 8.5 2.2 2.6 5.9 1.7 9.6 -2.2z m-19.9 -1.5 c7.7 -8.1 9.7 -18.8 3.6 -18.8 -4.2 0 -8.4 3.9 -11.5 10.7 l-3 6.4 2.3 2.5 c3 3.2 4.9 3 8.6 -0.8z m69.7 -130.1 c9.8 -4.5 22.4 -13.5 26.8 -19.1 5.4 -6.7 7.7 -12.1 9.4 -21.9 1.5 -8.3 3.1 -11.2 6.8 -12.3 4.3 -1.3 5.6 -3.4 5.6 -9 0 -6 -1.5 -7.7 -8.1 -8.9 -3.4 -0.6 -3.9 -0.4 -6.5 3.1 l-2.9 3.7 -13.5 -0.6 c-7.4 -0.4 -18.9 -1.3 -25.5 -2.1 -15.1 -1.8 -38.9 -3.1 -57 -3.1 -15.3 0 -17.6 0.7 -21.1 6 -2.7 4.2 -3 14.9 -0.6 21.8 3.4 9.5 16.8 26.7 26.9 34.5 12.1 9.4 18.8 11.3 38.8 10.9 14 -0.2 15.4 -0.4 20.9 -3z"/>
  <path d="M360.7 291.5 c-0.4 -5.8 -0.2 -7.5 1.4 -9.7 2.6 -3.5 2.4 -5.6 -0.6 -7 -1.9 -0.9 -2.9 -0.8 -4.8 0.5 -1.4 0.9 -2.7 1.3 -3.1 1 -1.2 -1.2 -0.6 -5.4 1.4 -9.3 2.2 -4.3 1.8 -6.9 -1.1 -7.1 -2.2 -0.2 -2.4 -0.2 -6 -1.2 -2.8 -0.7 -4.2 -3.6 -2.9 -5.7 0.9 -1.5 9.9 -4.7 15.6 -5.6 6.5 -1 14.2 1.2 21.9 6.1 3.8 2.5 7.7 4.5 8.6 4.5 5.2 0 6.3 -3 2.8 -7.4 -3.4 -4.3 0.4 -3.3 5.3 1.4 2.3 2.2 4.8 4 5.4 4 2.2 0 4.5 -3.1 3.9 -5.1 -0.9 -2.9 4 -2.5 7.7 0.6 3 2.5 7.2 3.3 8.3 1.5 0.7 -1.1 14.9 6.4 15.3 8.2 0.3 1.5 -8 5.6 -26.8 12.9 -7.4 3 -22.1 9.7 -32.6 14.9 l-19.2 9.6 -0.5 -7.1z"/>
  <path d="M548.1 233.3 c0.8 -1 3.1 -3.7 5.1 -5.9 4.7 -5.5 8.8 -12.6 9.9 -17.2 1.6 -6.9 -2.3 -13.2 -7.1 -11.7 -1.9 0.6 -2 0.1 -2 -7.7 l0 -8.3 8.8 8.7 c10.6 10.5 12.4 11.5 15.8 9.3 1.4 -0.9 3 -2.7 3.6 -4.1 0.6 -1.3 1.4 -2.4 1.8 -2.4 0.6 0 3.8 5 10.9 17 4.2 7.1 6.5 8.3 10.6 5.6 3.2 -2.1 3 -2.1 5.4 1.8 2.4 3.7 2.7 8.1 0.9 9.3 -0.7 0.4 -6 0.6 -11.8 0.4 -10.5 -0.2 -27.6 2 -40 5.4 -3 0.8 -7.3 1.5 -9.4 1.5 -3.6 0 -3.8 -0.1 -2.5 -1.7z"/>
  <path d="M528 173.2 c0 -3.3 1.6 -4.3 2.7 -1.6 0.3 0.8 -0.2 2.3 -1.1 3.2 -1.5 1.5 -1.6 1.4 -1.6 -1.6z"/>
 </g>
 <g fill="#eb7c31">
  <path d="M541.3 492.3 c-14.5 -12.9 -19.3 -16.6 -25 -19.4 -3.9 -1.9 -17.7 -10.1 -19.2 -11.5 -0.3 -0.3 0 -0.9 0.6 -1.5 0.7 -0.7 0.9 -4.1 0.6 -9.3 -0.5 -7.3 -0.1 -9.7 3 -21.2 2 -7.1 4.3 -16.8 5.3 -21.6 0.9 -4.9 2 -8.8 2.5 -8.8 1.1 0 4.7 4.7 5.4 7.1 0.9 2.6 3.6 2 5.3 -1.1 1 -1.9 1.2 -7.3 0.8 -23 -0.3 -11.3 -0.8 -22.2 -1.1 -24.2 -0.7 -3.7 -0.6 -3.8 2.2 -3.8 7.9 0 23.1 12.6 33 27.3 2.5 3.7 5 6.6 5.7 6.4 0.6 -0.2 1.2 -2 1.4 -4 0.4 -4.9 1.9 -4.8 11.7 0.9 4.4 2.6 10.1 5.9 12.8 7.3 6.2 3.4 6.9 5.6 2.8 8.9 -3 2.2 -3.1 2.6 -1.6 4.2 0.9 1 9.1 5.2 18.3 9.5 9.2 4.3 20.8 9.7 25.8 12 12.5 5.8 20.5 11.4 29.7 20.7 7.5 7.7 7.7 8 7.7 12.8 0 5.5 -1 6.2 -3.3 2.7 -2 -3.1 -10.6 -7.6 -16.1 -8.3 -5.2 -0.8 -24.5 1.2 -32.2 3.2 -6 1.5 -32 12.4 -38 16 -4.5 2.5 -19.3 18.4 -21.9 23.4 -0.9 1.7 -1.3 3.5 -1 4 0.3 0.6 -0.5 1 -1.9 1 -1.7 0 -5.7 -2.9 -13.3 -9.7z"/>
  <path d="M185.7 464.4 c-4.6 -9.2 -6.1 -18.7 -5.2 -32.4 1 -13.6 2.8 -21.7 7.6 -32.6 3.5 -8.1 4 -9.9 3.1 -12 -0.7 -1.3 -1.8 -2.4 -2.5 -2.4 -3.1 0 -8.5 2.6 -14.5 6.8 -3.5 2.4 -6.5 4.2 -6.7 4 -0.8 -0.8 4.2 -14.4 7.4 -20.5 1.7 -3.2 6.2 -9 10.1 -12.9 5.4 -5.6 6.8 -7.6 6.3 -9.3 -0.9 -3.5 -3.9 -4.5 -8.4 -2.7 -5 2 -10.9 1.9 -17.9 -0.2 -6.2 -2 -7.4 -4.1 -3.2 -6 5.1 -2.2 19.6 -14.6 21.9 -18.7 5.5 -9.7 4.8 -14.4 -1.7 -12.5 -1.9 0.6 -6.2 1 -9.4 1 l-5.9 0 5.9 -4.5 c3.2 -2.5 8.1 -6.4 10.7 -8.6 2.7 -2.3 4.9 -4 5.1 -3.8 0.1 0.2 0.8 4.8 1.4 10.2 1.7 14 3.8 23.7 7.8 35.5 1.9 5.7 3.4 10.6 3.4 10.8 0 0.3 -1.1 0.9 -2.5 1.4 -5.1 1.9 -5.9 11.3 -2.1 23.1 1.3 4.1 3.2 11 4.1 15.4 2 9.4 4.9 14.8 10.4 19.8 2.3 2 4.1 3.9 4.1 4.1 0 0.3 -3 4.9 -6.7 10.3 -8.8 13 -14.9 25.9 -17.1 36.1 l-1.8 8.2 -3.7 -7.6z"/>
  <path d="M398.6 453.4 c-4.7 -1.7 -10 -5.5 -13.9 -9.8 -2.3 -2.6 -11.7 -19.5 -11.7 -21.1 0 -0.9 21.3 -0.5 30 0.6 6.7 0.8 18.2 1.7 24.3 1.9 1.3 0 0.8 4.3 -1.6 13.1 -2.8 10.3 -4.7 13.6 -9.2 15.5 -4.3 1.8 -12.8 1.7 -17.9 -0.2z m-0.7 -16.8 c1.9 -2 2.1 -2.8 1.2 -4.5 -0.7 -1.1 -2.4 -2.7 -4 -3.5 -2.5 -1.3 -3.1 -1.3 -5.9 0.4 -3.8 2.4 -4 4.3 -0.7 7.5 3.1 3.2 6.5 3.2 9.4 0.1z"/>
  <path d="M599.5 377.2 c-4 -5.4 -3.9 -9.5 0.6 -20 2 -4.5 2.1 -4.6 3 -2.3 1.1 2.9 1.5 19.8 0.5 22.9 -0.9 2.8 -1.7 2.7 -4.1 -0.6z"/>
  <path d="M433.5 261.4 c-9.9 -5.2 -11.8 -6.6 -10.1 -7.6 2 -1.1 2.1 -5.7 0 -8.6 -1.8 -2.7 -7.5 -6.4 -12.1 -8 -2.7 -0.9 -3.6 -0.8 -4.8 0.4 -0.8 0.9 -1.5 2 -1.5 2.6 0 1.1 5.8 6.8 7 6.8 0.4 0 1.5 1.1 2.4 2.5 l1.6 2.5 -3.4 0 c-3 0 -3.7 -0.5 -5.2 -3.5 -2.2 -4.7 -10.4 -11.5 -14.8 -12.2 -6.7 -1.1 -9.2 4.4 -3.3 7.3 3.6 1.8 7.7 5.3 7.7 6.6 0 1.5 -2.7 0.8 -4.4 -1.1 -1 -1.1 -5 -3.5 -8.9 -5.5 -6.8 -3.3 -10.9 -4.3 -25.2 -6.5 -3.3 -0.5 -9.1 -1.7 -12.9 -2.7 l-6.9 -1.8 -2.9 -6.5 c-5.8 -12.8 -13.2 -23.5 -37.2 -54.1 -5.8 -7.4 -10.6 -13.9 -10.6 -14.4 0 -1.1 30 0 39 1.5 6.9 1.1 13.4 4.5 18.5 9.9 1.6 1.6 3.7 3 4.7 3 1.9 0 5.8 -3.3 5.8 -4.9 0 -0.5 -4.5 -5.8 -10 -11.8 -7.5 -8 -11.2 -13 -14.4 -19.3 -2.4 -4.7 -4.7 -9.5 -5.1 -10.8 -0.6 -2.2 -0.5 -2.1 4.5 0.8 5.8 3.4 18.2 12.5 22 16.1 1.5 1.4 5 5.5 7.9 9.2 2.9 3.7 5.9 6.7 6.7 6.7 4.2 0 3.8 -9.2 -1.2 -21.7 -4.4 -11.3 -15.4 -31.9 -21.1 -39.4 -2.9 -3.9 -5.3 -7.2 -5.3 -7.5 0 -0.9 11 4 17.5 7.8 13.2 7.8 34.7 23.3 40.5 29 4.8 4.9 7.7 9.6 10 16.3 2.2 6.3 3.2 7.5 6.7 7.5 3.4 0 4.1 -2.6 3.2 -12 -1.9 -18.4 -4.9 -31 -10 -41.2 -1.6 -3.2 -2.9 -6.2 -2.9 -6.7 0 -1.7 6.7 -2.2 11.1 -0.7 13 4.5 42.5 22.6 52.2 32.1 10.2 10 13.2 18.6 16.7 47.3 1.1 9.6 2.5 18.3 3.1 19.3 0.7 1.4 2.1 1.9 5.2 1.9 2.3 0 4.7 -0.4 5.4 -0.8 0.7 -0.4 1.5 -3.7 1.8 -7.2 0.9 -10.1 3.3 -20.3 8.2 -35.3 2.8 -8.7 4.9 -13.7 5.6 -13.4 2 0.6 3.8 17.2 3.3 29.7 -0.5 12.7 -2 18.9 -7.2 29.3 -1.9 3.7 -3.4 7.5 -3.4 8.4 0 2.5 3.1 4.2 7.8 4.3 3.7 0 4.9 -0.6 9.4 -4.7 5.5 -5.1 14.1 -17.5 15.2 -21.7 0.3 -1.4 1.1 -2.6 1.6 -2.6 1.7 0 2.3 4.9 1.8 14.8 -0.7 13.8 -3.6 20.3 -13.3 30.5 -4.1 4.3 -7.5 8 -7.5 8.2 0 2.1 2.8 5.7 4.9 6.1 1.4 0.3 3.1 0.7 3.6 0.8 2.4 0.6 -5.8 3.5 -15.5 5.5 -16 3.3 -34.2 8.3 -58.7 16.2 -16.5 5.3 -18.3 5.6 -22.8 3.3z"/>
 </g>
 <g fill="#ffffff">
  <path d="M660.2 799.4 c0.3 -0.3 2.3 -0.7 4.3 -1 5.2 -0.6 11.5 -5.7 11.5 -9.3 l0 -2.8 16.3 0.1 c31.8 0.2 34 -0.3 35.7 -7.9 1.2 -5.2 1.3 -10.5 0.1 -10.5 -0.6 0 -0.7 -1.6 -0.4 -3.5 0.3 -2 0.1 -4.1 -0.6 -4.8 -0.6 -0.8 -0.9 -1.6 -0.6 -1.9 0.9 -0.9 -2.4 -12.1 -3.7 -13 -0.8 -0.4 0.8 -0.8 3.7 -0.9 14.5 -0.2 20.8 -1.9 21.8 -5.8 0.3 -0.9 0.5 12.4 0.6 29.5 0.1 27.6 -0.1 31.3 -1.5 31.8 -2 0.8 -87.9 0.8 -87.2 0z"/>
  <path d="M643 787.3 c-4.9 -1 -12.7 -4.6 -23 -10.8 -12.6 -7.7 -19.5 -13.6 -24.4 -21 -3.3 -4.9 -4.1 -5.5 -7.1 -5.5 -3.9 0 -5.8 2.5 -7 9.3 -0.4 2 -1.1 3.9 -1.5 4.2 -0.4 0.3 -3.9 -2.6 -7.6 -6.3 -16.2 -16 -47.5 -33.6 -72.3 -40.7 -9.7 -2.8 -34.3 -7.9 -50.1 -10.4 l-10.5 -1.7 -3.9 -6.8 c-3 -5.2 -4 -8 -4 -11.5 l-0.1 -4.6 9 -0.7 c15.7 -1.2 16.5 -1.3 16.2 -2.1 -0.1 -0.5 -5.4 -4.6 -11.7 -9 -9.3 -6.6 -12.3 -9.4 -15.8 -14.6 -2.4 -3.5 -4.1 -7.1 -4 -8 0.5 -2.4 16.8 -3.6 27.1 -2 11.6 1.7 11.9 1 5.7 -11.6 -2.7 -5.5 -4.9 -10.3 -5 -10.7 0 -0.4 2.2 -1.9 4.9 -3.3 5.8 -3.2 7.8 -6.3 6.2 -9.4 -1.6 -2.9 -4.4 -2.6 -11.1 1 l-5.7 3.2 -5.4 -2.6 c-3 -1.4 -9.6 -3.7 -14.7 -5.2 -16.8 -4.8 -24.1 -8.3 -34.5 -16.3 -6.1 -4.8 -9.7 -9.6 -9.7 -13.1 0 -2 0.5 -2.1 8 -2.1 7.7 0 8 -0.1 7 -2 -0.6 -1 -5.2 -6.4 -10.3 -11.8 -9.3 -9.9 -19.7 -24.1 -19.7 -26.8 0 -1.6 -0.7 -1.6 13.3 -1.4 5.9 0.1 10.7 -0.3 10.7 -0.7 0 -0.4 -3.6 -4.8 -8 -9.8 -4.4 -4.9 -8 -9.5 -8 -10.1 0 -0.6 1.5 -1.9 3.4 -2.8 2.6 -1.2 6.5 -1.6 16.9 -1.6 28.3 0 47.9 -6.5 75.8 -25.2 11.6 -7.7 20.8 -15.7 25.4 -22 l2 -2.8 7 4.5 c3.9 2.5 9.7 5.9 13.1 7.6 5.6 2.8 18.1 12.8 31.5 25.3 5 4.7 5.8 6 7.3 12.1 2 8.2 5.8 16.4 9.4 20.7 1.8 2.1 7.2 5.3 15.8 9.4 18 8.5 24.6 10.5 37.9 11.1 26.2 1.3 36.5 -7.6 47.5 -41.2 2.9 -8.8 3.4 -11.4 3.4 -21 0.1 -10.6 0 -11.3 -3.2 -17.4 -2.7 -5.2 -3.2 -7.3 -3 -11.5 l0.3 -5.1 2.9 4.5 c5.3 8.1 7 15.1 7 28.5 0 14.4 -3.7 36.4 -8.4 50 -1.7 5 -3.6 10.6 -4.2 12.5 -1.8 6.2 -4 7.6 -16.7 10.5 -14.4 3.4 -21.5 6.2 -43 17 -20.8 10.4 -28.5 12.7 -42.4 12.6 -12.1 0 -20.9 -2.3 -39.2 -10.1 -27.8 -11.9 -42.6 -13.5 -64.4 -7 -3.8 1.1 -7.7 2.8 -8.6 3.6 -1.6 1.7 -1.2 5.2 0.9 6.7 1.4 1 13.7 0 19.1 -1.5 5.8 -1.6 19 -1.3 26.5 0.6 3.6 1 11.9 4.1 18.5 7.1 19.9 8.9 29.9 11.6 47.5 13 13.9 1.1 19.6 -0.7 46.8 -14.1 20 -9.9 24.6 -11.8 30.6 -12.7 9.3 -1.2 13.1 0 12.2 4.1 -1.3 5.3 -6.7 15.1 -11.1 20 -12.2 13.6 -31.6 19.7 -66 20.8 -14.9 0.5 -23.2 0.2 -48 -2.1 -16.5 -1.4 -35.5 -2.7 -42.3 -2.8 -11.2 -0.2 -12.5 0 -13.8 1.7 -0.8 1.1 -1.4 2.3 -1.4 2.8 0 2.1 4.2 4.6 8.8 5.3 2.8 0.4 5.6 1.2 6.1 1.9 0.6 0.7 2.1 4.5 3.5 8.4 6.4 18.9 12.9 23.6 34.6 24.9 10.3 0.6 15 1.7 13.4 3.3 -0.5 0.4 -4 1.8 -7.9 3 -8 2.6 -9.5 3.5 -9.5 6 0 3.8 2.7 4.5 9.5 2.7 8.2 -2.2 21.6 -2.2 27.9 -0.1 4 1.4 4.6 1.9 3.4 3 -0.7 0.8 -4.5 2.4 -8.2 3.6 -7.9 2.6 -10.3 5.1 -7.1 7.4 1.1 0.9 2.9 1.2 4 0.9 1.1 -0.4 7 -0.9 13.1 -1.2 13.2 -0.6 20 1.1 32.4 8 4.1 2.4 11.5 6.2 16.3 8.6 6 3 10.7 6.2 14.5 10.1 6.4 6.3 20.5 17 22.5 17 0.7 0 2.1 -0.6 3.1 -1.3 1.6 -1.2 1.2 -2.1 -4.8 -11.7 -9.9 -15.8 -15.5 -31 -15.6 -42.2 0 -6.2 -1.6 -9 -4.7 -8.6 -1.7 0.2 -3.2 1.2 -3.9 2.6 -0.6 1.2 -1.4 2.5 -1.7 2.9 -1.4 1.3 -3.7 -6.9 -3.7 -13.1 0 -5.7 -0.3 -6.7 -2.2 -7.7 -1.8 -1 -2.4 -0.8 -4 1.1 -1 1.2 -2.3 4 -3 6.1 -1.3 4.5 -3.6 4.9 -5.5 0.9 -1.7 -3.6 -3 -14.2 -2.9 -23.4 l0.1 -8 11 -2.2 c13.5 -2.9 21.2 -5.6 28.8 -10.1 l5.8 -3.5 3.9 3.6 c4.8 4.3 12.2 8.1 18.7 9.7 2.6 0.7 13.3 1.5 23.7 1.8 l18.9 0.6 -2.7 3.2 c-4 4.7 -17.3 10.9 -30.3 14.1 -6.2 1.5 -11.8 3.3 -12.4 3.9 -1.8 1.8 -0.3 5.8 2.6 7.1 1.6 0.7 6.9 1.2 12.8 1.1 11.4 -0.1 12.5 0.1 25.2 4.2 14 4.6 14.7 5.5 4.6 6.6 -4.6 0.4 -7.1 1.3 -9.2 3 -4.9 4.1 -3.7 8.3 3.9 13.5 12.1 8.3 22.2 20.3 29.3 35.1 3.4 7.2 3.9 9 2.9 10.2 -1.5 1.8 -11 2 -17.4 0.3 -3.8 -1 -4.7 -0.9 -7.7 0.8 -5.4 3.2 -5.8 6.7 -1.6 15 4.6 9.3 8.3 23 6.6 25 -0.9 1.1 -4.2 1.4 -15.3 1.4 -17.8 0 -25 -1.1 -35.5 -5.1 -12.6 -4.9 -15.2 -1.9 -8.2 9.7 2.3 3.7 4.1 7.1 4.1 7.5 0 2.1 -10.5 3.4 -17 2.2z m24.6 -43.3 c1.9 -5.2 1.8 -13.9 -0.2 -14.7 -0.8 -0.3 -2 -0.1 -2.5 0.4 -1.4 1.4 -3.4 14.6 -2.6 16.7 1.1 2.7 3.9 1.4 5.3 -2.4z m-8 -2.2 c0.9 -1.4 0.9 -13.2 0 -14.7 -1.3 -2 -3.6 -1.2 -4.7 1.7 -1.5 3.7 -1.3 9 0.6 11.8 1.7 2.6 3 3 4.1 1.2z"/>
  <path d="M620 702.9 c-13 -8.6 -26.4 -15.3 -39.5 -19.5 -9.9 -3.2 -10 -3.3 -10 -6.4 0 -3.2 -3.3 -7 -6.1 -7 -0.7 0 -2.8 -0.6 -4.6 -1.4 -1.8 -0.8 -5 -1.8 -7 -2.1 -3.7 -0.6 -3.8 -0.7 -3.8 -5 l0 -4.4 -5.2 -1.6 c-2.9 -0.9 -10.9 -2 -17.8 -2.5 -21.1 -1.5 -27 -5 -31.9 -19.1 -1 -2.9 -2.2 -6.3 -2.7 -7.3 -0.6 -1.7 -0.4 -1.8 2.7 -1.3 1.9 0.4 16.7 1.8 32.9 3.3 27.5 2.4 46.5 2.8 62 1.2 l2.5 -0.3 0.5 14.5 c0.6 14.4 0.6 14.6 4.4 22.3 4.6 9.2 6.7 10.9 10 7.8 1.2 -1.2 2.5 -2.1 2.8 -2.1 0.3 0 2.2 3.3 4.1 7.4 3.4 7.1 3.7 7.4 6.8 7.3 3 -0.2 3.3 0.1 4.4 4.3 0.7 2.5 2.6 7.8 4.3 11.8 1.7 3.9 2.8 7.2 2.4 7.2 -0.4 0 -5.4 -3.2 -11.2 -7.1z"/>
  <path d="M161.2 703.1 c-1.7 -1 -0.5 -6.1 1.4 -6.1 1.8 0 3.6 3.4 2.8 5.4 -0.7 1.8 -2.2 2 -4.2 0.7z"/>
  <path d="M404 470.7 c-0.3 -0.2 -3.1 -0.7 -6.4 -1.1 -7.7 -0.8 -13.7 -4.2 -22.5 -12.7 -8.8 -8.5 -15.1 -17 -18.1 -24.3 -2.1 -5.3 -2.1 -5.7 -0.6 -8.2 2.8 -4.2 5.5 -2.9 7.5 3.6 4.4 14.3 14.6 27.3 25.3 32.3 6.5 3.1 15.8 5.7 20.3 5.7 9.1 0 19.5 -6.2 23.5 -13.9 2 -4 5.7 -17 6.2 -21.6 l0.3 -3 9 0 9 0 -0.2 4.8 c-0.4 13.2 -14.7 29.4 -31.3 35.8 -5 1.9 -20.8 3.8 -22 2.6z"/>
  <path d="M388.3 441.6 c-6.9 -3.1 -8 -12 -2 -15.7 3.8 -2.3 7.8 -2.4 11.7 -0.4 3.6 1.9 5 4.3 5 8.8 0 6.4 -7.9 10.4 -14.7 7.3z"/>
  <path d="M603.6 392.8 c-2.2 -3.1 -2 -3.8 1.2 -3.8 1.7 0 3.7 -1 5 -2.3 2 -2.1 2.3 -3.8 2.8 -16.3 0.6 -12.6 0.4 -14.4 -1.5 -19.4 -1.1 -3 -2.1 -6.8 -2.1 -8.4 0 -2.9 4.4 -11.6 5.9 -11.6 1.6 0 6.1 23.8 6.1 32.8 0 7 -2.5 17.7 -5.3 22.7 -2 3.5 -7.7 8.5 -9.8 8.5 -0.4 0 -1.5 -1 -2.3 -2.2z"/>
 </g>
</svg>
</body>
</html>
)=====";
const char MainPage[] PROGMEM = R"=====(
  <!DOCTYPE html>
  <html>
   <head>
       <title>WIFI CONFIG</title>
       <style>
          body {text-align:center;}
          h1 {color:#003399;}
          a {text-decoration: none;color:#FFFFFF;}
          .bt_off {height:50px; width:100px; margin:10px 0;background-color:#FF6600;border-radius:5px;}
          .bt_on {height:50px; width:100px; margin:10px 0;background-color:#00FF00;border-radius:5px;}
          .bt_write {height:30px; width:70px; margin:10px 0;background-color:#00FF00;border-radius:5px;}
          .bt_restart {height:30px; width:70px; margin:10px 0;background-color:#FF6600;border-radius:5px;}
          .bt_clear {height:30px; width:70px; margin:10px 0;background-color:#00FF00;border-radius:5px;}
          .bt_update {height:50px; width:90px; margin:10px 0;background-color:#3273DB;border-radius:5px;}
          .left_p {text-align: left; margin-left: 10px; color:DodgerBlue;}
          .center_p {text-align: center; margin-left: 10px; color:DodgerBlue;}
       </style>
       <meta name="viewport" content="width=device-width,user-scalable=0" charset="UTF-8">
   </head>
   <body>
      <h1>IoTVision Web Server</h1>
      <div>Thiết lập Wifi</div>
      <div class = "center_p">Tên Wifi: <input id="ssid"/></div>
      <div class = "center_p">MK Wifi: <input id="pass"/></div>
      <div>
        <button class="bt_write" onclick="writeEEPROM()">KẾT NỐI</button>
        <button class="bt_write" onclick="scanWifi()">SCAN</button>
        <button class="bt_restart" onclick="restartESP()">RESTART</button>
        <button class="bt_clear" onclick="clearEEPROM()">XOÁ</button>
      </div>
      <div>Temp: <pan id="temp"></pan></div><div> Humi: <pan id="humi"></pan></div>
      <div id>IP connected: <pan id="ipconnected"></pan></div>
      <div id>Time: <pan id="timeapi"></pan></div>
      <div id="reponsetext"></div>
      <script>
        //-----------Tạo đối tượng request----------------
        function create_obj(){
          td = navigator.appName;
          if(td == "Microsoft Internet Explorer"){
            obj = new ActiveXObject("Microsoft.XMLHTTP");
          }else{
            obj = new XMLHttpRequest();
          }
          return obj;
        }
        var xhttp = create_obj();
        //------------------------------------------------
        window.onload = function(){
          xhttp.open("GET","/getIP",true);
          xhttp.onreadystatechange = process_ip;//nhận reponse
          xhttp.send();
        }
        //-----------Kiểm tra response IP------------------
        function process_ip(){
          if(xhttp.readyState == 4 && xhttp.status == 200){
            //------Updat data sử dụng javascript----------
            ketqua = JSON.parse(xhttp.responseText);
            document.getElementById("ipconnected").innerHTML=ketqua.ip;
            document.getElementById("timeapi").innerHTML=ketqua.datetime;
            document.getElementById("ssid").innerHTML=ketqua.ssid;
            document.getElementById("pass").innerHTML=ketqua.pass;
          }
        }
        //==============================================================
        //---Ham update duu lieu tu dong---
        var xhttp2 = create_obj();
        var connected = false;
        setInterval(function() {
          get_timeAPI();
        }, 500);// 300ms van OK
        function get_timeAPI(){
          xhttp2.open("GET","/get_timeAPI",true);
          xhttp2.onreadystatechange = process_api;//nhận reponse
          xhttp2.send();
        }
        function process_api(){
          if(xhttp2.readyState == 4 && xhttp.status == 200){
            //------Updat data sử dụng javascript----------
            ketqua = JSON.parse(xhttp2.responseText);
            document.getElementById("timeapi").innerHTML=ketqua.datetime;
            document.getElementById("temp").innerHTML=ketqua.temp;
            document.getElementById("humi").innerHTML=ketqua.humi;
          }
        }
        //===============================================================
        //-----------Thiết lập dữ liệu và gửi request ssid và password---
        function writeEEPROM(){
          var ssid = document.getElementById("ssid").value;
          var pass = document.getElementById("pass").value;
          xhttp.open("GET","/writeEEPROM?ssid="+ssid+"&pass="+pass,true);
          xhttp.onreadystatechange = process_reponse;//nhận reponse
          xhttp.send();
        }
        function clearEEPROM(){
          xhttp.open("GET","/clearEEPROM",true);
          xhttp.onreadystatechange = process_reponse;//nhận reponse
          xhttp.send();
        }
        function restartESP(){
          xhttp.open("GET","/restartESP",true);
          xhttp.onreadystatechange = process_reponse;//nhận reponse
          xhttp.send();
        }
        function scanWifi(){
          xhttp.open("GET","/scanWifi",true);
          xhttp.onreadystatechange = process_reponse;//nhận reponse
          xhttp.send();
        }
        function updateOTA(){
          xhttp.open("GET","/OTA",true);
          xhttp.onreadystatechange = process_reponse;//nhận reponse
          xhttp.send();
        }
        //-----------Kiểm tra response ------------------
        function process_reponse(){
          if(xhttp.readyState == 4 && xhttp.status == 200){
            //------Updat data sử dụng javascript----------
            ketqua = xhttp.responseText;
            document.getElementById("reponsetext").innerHTML=ketqua;
          }
        }
      </script>
   </body>
  </html>
)=====";

#endif