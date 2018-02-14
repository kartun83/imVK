//.pragma library

function fmtTime(inSeconds)
{
	if (inSeconds == 0) { return "";}
    var a = new Date(inSeconds*1000);
    var months = ['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'];
    var day_names = ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'];
    var year = a.getFullYear();
    var month = months[a.getMonth()];
    var date = a.getDate();
    var c_date = new Date(); // Current date        
    var days = Math.floor((c_date - a) / 1000 / 60 / (60 * 24));    
    var c_year = c_date.getFullYear();
    var c_month = months[c_date.getMonth()];
    var c_date2 = c_date.getDate();
    var hour; var min; var sec;
    if (a.getHours()   < 10) {hour = "0"+a.getHours();}   else { hour = a.getHours();  }
    if (a.getMinutes() < 10) {min  = "0"+a.getMinutes();} else { min  = a.getMinutes();}
    if (a.getSeconds() < 10) {sec  = "0"+a.getSeconds();} else { sec  = a.getSeconds();}
  
    // Fix for invalid clock settings on device
    if (days < 7 && days >= 0)
      {
        time = day_names[days] + ', ' + hour + ':' + min + ':' + sec ;
      }
    else
      {
	    var time;
	    if (c_year == year &&  c_date2 == date && c_month == month)
	    {                        
	        time = hour + ':' + min + ':' + sec ;
	    }
	    else 
	    {
	    	if (c_year == year)
	    		time = date + ',' + month + ' ' + hour + ':' + min + ':' + sec ;
	    	else
	    		time = date + ',' + month + ' ' + year + ' ' + hour + ':' + min + ':' + sec ;
	    }
    }
    return time;
}

function fmtDuration(inSeconds)
{
    var sec_num = parseInt(inSeconds, 10); // don't forget the second param
    var hours   = Math.floor(sec_num / 3600);
    var minutes = Math.floor((sec_num - (hours * 3600)) / 60);
    var seconds = sec_num - (hours * 3600) - (minutes * 60);

    if (hours   < 10) {hours   = "0"+hours;}
    if ( (minutes < 10) && (hours > 1) )  { minutes = "0"+minutes;} 
    if (seconds < 10) {seconds = "0"+seconds;}
    var time;
    
    if (hours > 1)
    {
    	time    = hours+':'+minutes+':'+seconds;
    }
    else
    {
    	time    = minutes+':'+seconds;
    }
    return time;
}

function timeFromNow(inSeconds)
{
	var a = new Date().getTime();
	var milisec_diff = ( a - ( inSeconds * 1000) );	
	
	var days = Math.floor(milisec_diff / 1000 / 60 / (60 * 24));

    var date_diff = new Date( milisec_diff );
    var result;
    var hours   = date_diff.getHours();
    var minutes = date_diff.getMinutes(); 
    if ( days > 0)
    {
    	result = days + qsTr(" Days ");// + date_diff.getHours() + qsTr(" Hours ") + date_diff.getMinutes() + qsTr(" Minutes ");
    } else
    	if (hours > 0)
    		{
    			result = hours + qsTr(" Hours ") + minutes + qsTr(" Minutes ");
    		}
    	else if (minues > 0)
    	{
    		result = minues + qsTr(" Minutes ");
    	}
    	else
    	{
    		result = qsTr("just now");
    	}
    return result; // + date_diff.getSeconds() + qsTr(" Seconds");
	
}

function parseUrl(querystring) {
    // remove any preceding url and split
    querystring = querystring.toString().substring(querystring.toString().indexOf('#')+1).split('&');
    var params = {}, pair, d = decodeURIComponent;
    // march and parse
    for (var i = querystring.length - 1; i >= 0; i--) {
        pair = querystring[i].split('=');
        params[d(pair[0])] = d(pair[1]);
    }
    
    return params;
} //--  fn  deparam