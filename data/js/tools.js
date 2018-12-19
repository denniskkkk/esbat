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
window.onload = function () {
var labels = [];
var dataSamples = [];
var ctx = document.getElementById("enchart").getContext('2d');
var chartdata = [];
var hst = [];
chartdata.labels = labels; 
for (z = 0; z < allch; z++) {	
   chartdata[z] =  {
		label : 'CH' + (z + 1) ,
		pointRadius : 1,
		lineTension : 0,
		borderWidth : 1,
        lineWidth : 1,
		XAxisID : 'timelog',
        //YAxisID : 'levellog',
		fill : false,
		backgroundColor : chartcolors [z % 6],
		borderColor : chartcolors [z % 6],
		data : [] };
}
var tbl = $("table#tsta");
var tbb = $("table#tdat");
for (q =0; q < allch / 10 ; q ++){
var row = $('<tr></tr>').appendTo(tbl);
for (i =0; i < 10; i++ ) {
   $('<td></td>').html("<div id='xch" + (q *10 + i) + "'>N/A</div>").appendTo(row);
   $("div#xch" + (q *10 + i)).attr("style", "background-color:GREY;");
}
}
for (q =0; q < allch / 20 ; q ++){
	var drow = $('<tr></tr>').appendTo(tbb);
	for (i =0; i < 20; i++ ) {
	   $('<td></td>').html("<div id='dch" + (q * 20 + i) + "'>N/A<br>.</div>").appendTo(drow);
	   $("div#dch" + (q * 20 + i)).attr("style", "background-color:GREY;");
	}
}

var enchart = new Chart(ctx, {
	type : 'line',
	data : chartdata ,
	data : {
		labels : labels,
        datasets :  chartdata
	}, 
	options : {
		animation : {
			duration : 0
		},
		scales : {
			xAxes : [ {
				id : 'timelog',
				display : true,
				scaleLabel: {
					display: true,
					labelString: 'Sample Datetime (Format:JAN-01 1:59:01)',
					fontSize : 10
				},
				ticks : {
					maxTicksLimit: 50,
					callback : function(value, index, values) {
						 if (value != null) {
						      return moment(value).format("MMM-DD h:mm:ss");
						 } else {
						 return "-";
						 }
					}
				}
			} ],
			yAxes : [ {
				id : 'levellog',
				display : true,
				scaleLabel: {
					display: true,
					labelString: 'Measure in Volt',
					fontSize : 10
				},
				type : 'linear',
				fontSize : 10,
				ticks : {
					min : 0,
					max : 20000,
					scaleSteps : 1,
					scaleStartValue : 0,
					stepSize : 1000,
					callback : function(value, index, values) {
						return value ;
					}
				}
			} ]
		},legend: {
            display: true,
            labels: {
                fontSize: 10
            }
        },
        layout: {
        	padding: {
        		left: 0,
        		right: 0,
        		top: 0,
        		bottom: 0
        		}
        },				
        tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},		
        title: {
            display: true,
            text: 'DATA LOGGING GRAPH'
        }
	}
});
function datasample(value, st, d) {
	this.value = value;
	this.datetime = d;
	this.st = st;
}
var v = [];
var st = [];
for (q = 0; q < allch; q++) {
		v[q] = null;
		st[q] = "";
	}
var d = null;
var logcnt = 0;
var ndata = new datasample(v, st, d);
var fails = [];
dataSamples = Array.apply(null, Array (slen  )).map (()=> { return ndata });
hst = Array.apply (null, Array (allch)).map ((m, i) => { return basead + (201 + i) });
fails = Array.apply (null, Array (allch)).map (() => {return 0;});
function adddata( v , sta) {
	var date = new Date();
    var dateTime = [];	
	var datas = new datasample(v, sta, new Date ());
    for (i = dataSamples.length - wid; i < dataSamples.length   ; i++) {
	     dateTime.push (dataSamples[i].datetime);  
    }		
	dataSamples.push(datas);
	logcnt ++;
	for (q = 0; q < allch ; q++) {
		var ch = Array.apply (null, Array (wid )).map (() => { 
			var vtmp = [];
			for (i = dataSamples.length - wid  ; i < dataSamples.length ; i++) {
				vtmp.push (dataSamples [i].value[q]);
			}
			return vtmp;
			});
			enchart.data.datasets[q].data = ch[q];
			if (sta[q] =="y") {
		      $("div#xch" + q).html("CH" + (q+1) + " : " + fails[q] + "/" + logcnt);
		      $("div#xch" + q).attr("style", "background-color:CYAN;");
			} else {
			  $("div#xch" + q).html("CH" + (q+1) + " : " + fails[q] + "/" + logcnt);
		      $("div#xch" + q).attr("style", "background-color:RED;");
			}
	}
    for (q =0; q < allch ; q ++){
    if (v[q] != null) {
       $("div#dch" + q ).html("CH" + (q+1) + "<br>" + v[q] + "V");
       $("div#dch" + q ).attr("style", "background-color:WHITE;");
    } else {
       $("div#dch" + q ).html("CH" + (q+1) + "<br>" + "N/A");
       $("div#dch" + q ).attr("style", "background-color:GREY;");
    }
    }
	enchart.data.labels = dateTime;
	enchart.update();
    setTimeout(() => {loaddata () }, tout3);
}

function loaddata () {
   var v = [];
   var s = [];
   v = Array.apply ( null, Array (allch)).map (() => { return null; });
   s = Array.apply ( null, Array (allch)).map (() => { return "-"});
   hst.forEach ( (m, i) => {
	  var u = "http://" + m + "/data/";
	  $.ajax ({
      url: u ,
      method: 'GET',
      cache: false,
      datatype: 'application/json',
      crossOrigin: true,
      timeout: tout2,
      success: (data) => {
         v[i] = data.ch0 * 1000;
         s[i] = "y";
         },
      error: ( ) => {
    	 v[i] = null;
         s[i] = "n";
         fails[i] ++;
      }
     });
   });
   chkcomp (v, s);
}

var lcnt = 0
function chkcomp (v, s ) {
	var chkcnt = 0;
	lcnt ++;
	if (dataSamples.length > slen ) {
		dataSamples.shift();
	}
    s.forEach( (m, i) => {
		   if ( m === 'y' || m === 'n') chkcnt ++;
	   });
	if (chkcnt >= allch) {
		   lcnt = 0;
		   adddata (v, s );
		   return;
	   } else {
		   if (lcnt > 30) {
			   lcnt = 0;
			   adddata (v, s);
			   return;
		   }
		   setTimeout (()=> { chkcomp (v ,s);} , 100);
	   }
}
loaddata();
}
