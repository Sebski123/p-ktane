const char INDEX_HTML[] =
"<!DOCTYPE HTML>"
"<html>"
""
"<head>"
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0\">"
"    <title>KTANE SETUP</title>"
"    <style>"
"        body {"
"            background-color: #101010;"
"            font-family: Arial, Helvetica, Sans-Serif;"
"            Color: #808080;"
"        }"
"    </style>"
"</head>"
""
"<body>"
"    <h1>KTANE setup</h1>"
"    <FORM action=\"/\" method=\"post\">"
"        <P>"
"            <b>Configure external features</b><br><br>"
"            Serial Number: <input type=\"text\" class=\"field left\" readonly name=\"serial_num\" id=\"serial_num\">"
"            <input type=\"button\" value=\"Generate Serial\" onclick=\"generateSerial()\">"
"            <BR>"
"            Wires: <input type=\"text\" class=\"field left\" readonly name=\"wires_num\" id=\"wires_num\" size=\"29\">"
"            <input type=\"button\" value=\"Generate Wires\" onclick=\"generateWires()\">"
"            <BR>"
"            Defuse time in minutes:"
"            <select name=\"num_minutes\">"
"                <option value=\"0\">0</option>"
"                <option value=\"1\">1</option>"
"                <option value=\"2\">2</option>"
"                <option value=\"3\">3</option>"
"                <option value=\"4\">4</option>"
"                <option value=\"5\">5</option>"
"                <option value=\"6\" selected>6</option>"
"                <option value=\"7\">7</option>"
"                <option value=\"8\">8</option>"
"                <option value=\"9\">9</option>"
"            </select><BR>"
"            <!--Number of batteries:"
"            <select name=\"num_batteries\">"
"                <option value=\"0\">0</option>"
"                <option value=\"1\">1</option>"
"                <option value=\"2\">2</option>"
"                <option value=\"3\">3</option>"
"                <option value=\"4\">4</option>"
"                <option value=\"5\">5</option>"
"                <option value=\"6\">6</option>"
"                <option value=\"7\">7</option>"
"            </select><BR>"
"            Other items: <br>"
"            <input type=\"checkbox\" name=\"port1\" value=\"port1\"> Lit FRK indicator<br>"
"            <input type=\"checkbox\" name=\"port2\" value=\"port2\"> Lit CAR indicator<br>-->"
"            <input type=\"checkbox\" name=\"port3\" value=\"port3\"> Parallel port<br>"
"            <input type=\"checkbox\" name=\"port4\" value=\"port4\"> RJ45 port<br>"
"            <input type=\"checkbox\" name=\"port5\" value=\"port5\"> Stereo RCA port<br>"
"            <br>"
"            <INPUT type=\"submit\" value=\"Send\"> <INPUT type=\"reset\">"
"        </P>"
"    </FORM>"
"    <script>"
"        function randomString(length, chars) {"
"            var result = \"\";"
"            for (var i = length; i > 0; --i) result += chars[Math.round(Math.random() * (chars.length - 1))];"
"            return result;"
"        }"
""
"        function generateSerial() {"
"            let serial = randomString(5, \"0123456789ABCDEFGHIJKLMNOPQRSTUVWXZ\");"
"            let number = Math.round(Math.random() * (9)).toString();"
"            console.log(number);"
"            serial += number;"
"            console.log(serial);"
"            document.getElementById(\"serial_num\").value = serial;"
"            Math.seedrandom(serial);"
"        }"
""
"        function generateWires() {"
"            if (document.getElementById(\"serial_num\").value == \"\") {"
"                generateSerial()"
"            }"
"            let wireNum = Math.floor(Math.random() * 4) + 3;"
"            const colorArray = [\"Yellow\", \"Red\", \"Blue\", \"Black\", \"White\"];"
"            colorCombo = [];"
"            console.log(wireNum, colorCombo);"
"            while (wireNum >= 0) {"
"                colorCombo.push((colorArray[Math.round(Math.random() * (colorArray.length - 1))])) + \",\";"
"                wireNum--;"
"            }"
"            colorCombo.pop();"
"            document.getElementById(\"wires_num\").value = colorCombo;"
"        }"
"        "
"        /*"
"        seedrandom.js"
"        ============="
""
"        Seeded random number generator for Javascript."
""
"        version 2.3.10"
"        Author: David Bau"
"        Date: 2014 Sep 20*/"
"        !function(a,b,c,d,e,f,g,h,i){function j(a){var b,c=a.length,e=this,f=0,g=e.i=e.j=0,h=e.S=[];for(c||(a=[c++]);d>f;)h[f]=f++;for(f=0;d>f;f++)h[f]=h[g=s&g+a[f%c]+(b=h[f])],h[g]=b;(e.g=function(a){for(var b,c=0,f=e.i,g=e.j,h=e.S;a--;)b=h[f=s&f+1],c=c*d+h[s&(h[f]=h[g=s&g+b])+(h[g]=b)];return e.i=f,e.j=g,c})(d)}function k(a,b){var c,d=[],e=typeof a;if(b&&\"object\"==e)for(c in a)try{d.push(k(a[c],b-1))}catch(f){}return d.length?d:\"string\"==e?a:a+\"\\0\"}function l(a,b){for(var c,d=a+\"\",e=0;e<d.length;)b[s&e]=s&(c^=19*b[s&e])+d.charCodeAt(e++);return n(b)}function m(c){try{return o?n(o.randomBytes(d)):(a.crypto.getRandomValues(c=new Uint8Array(d)),n(c))}catch(e){return[+new Date,a,(c=a.navigator)&&c.plugins,a.screen,n(b)]}}function n(a){return String.fromCharCode.apply(0,a)}var o,p=c.pow(d,e),q=c.pow(2,f),r=2*q,s=d-1,t=c[\"seed\"+i]=function(a,f,g){var h=[];f=1==f?{entropy:!0}:f||{};var o=l(k(f.entropy?[a,n(b)]:null==a?m():a,3),h),s=new j(h);return l(n(s.S),b),(f.pass||g||function(a,b,d){return d?(c[i]=a,b):a})(function(){for(var a=s.g(e),b=p,c=0;q>a;)a=(a+c)*d,b*=d,c=s.g(1);for(;a>=r;)a/=2,b/=2,c>>>=1;return(a+c)/b},o,\"global\"in f?f.global:this==c)};if(l(c[i](),b),g&&g.exports){g.exports=t;try{o=require(\"crypto\")}catch(u){}}else h&&h.amd&&h(function(){return t})}(this,[],Math,256,6,52,\"object\"==typeof module&&module,\"function\"==typeof define&&define,\"random\");"
"    </script>"
"</body>"
""
"</html>";