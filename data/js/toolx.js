/*
MIT License

Copyright (c) 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/* log monitor console
 * Written by Dennis Ho 2018, July
 * 
 */
var chartcolors = [
		 'rgb(255, 99, 132)',
		 'rgb(255, 159, 64)',
		 'rgb(255, 205, 86)',
		 'rgb(75, 192, 192)',
		 'rgb(54, 162, 235)',
		 'rgb(153, 102, 255)',
		 'rgb(201, 203, 207)'
];
		
var slen = 5000;
var wid = 500;
var allch = 40
var basead = "192.168.1.";
// time = timeout + resttime
var tout2 = 5000; // timeout
var tout3 = 1000; // resttime
