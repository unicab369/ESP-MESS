char HTTP_WEB_START[] = R"!^!(<!DOCTYPE html>
<html><head>
<script src='root.js'></script>
<link rel='stylesheet' href='button.css'>
<link rel='stylesheet' href='forms.css'>
</head><body>
<div style='margin: auto; width: 640px; background-color: MediumSeaGreen;'>
)!^!";

char HTTP_WEB_END[] = "</div></body></html>";

char MAIN_JS2[] = R"!^!(
function goBack() {
   console.log('Button clicked');
   window.history.back();
}
function showAlert() {
   var xhr = new XMLHttpRequest();
   xhr.onreadystatechange = function() {
      if (xhr.readyState === 4 && xhr.status === 200) {
         alert('Read Value: ' + xhr.responseText);
      }
   };
   xhr.open('GET', '/analogRead', true);
   xhr.send();
}
function sendPostRequest(path, isPost) {
   console.log('IM HERE sendPostRequest');
   var xhr = new XMLHttpRequest();
   xhr.onreadystatechange = function() {
      if (xhr.readyState === 4 && xhr.status === 200) {
      }
   };
   xhr.open(isPost ? 'POST' : 'GET', path, true);
   xhr.send();
}
)!^!";



#define HTML_BUFFER_SIZE 1024

class Web_Base: public Loggable {
   private:
      char* buffer = new char[HTML_BUFFER_SIZE];
      int bufferSize = HTML_BUFFER_SIZE;

   void appendStr(const char* str) {
      int currentLen = strlen(buffer);
      int appendLen = strlen(str);

      if (currentLen + appendLen >= bufferSize) {
         // Calculate the new buffer size (e.g., double the size)
         bufferSize = (currentLen + appendLen + 1) * 2; // +1 for the null terminator

         // Allocate a new buffer and copy the existing content
         char* newBuffer = new char[bufferSize];
         strcpy(newBuffer, buffer);    

         delete[] buffer;        // Clean up the old buffer
         buffer = newBuffer;     // Update the buffer pointer          
      }
      
      strcat(buffer, str);   // Append the additional content
   }

   protected:
      virtual void makeContent() {}
      char dirPath[20];
      Serv_Network* network;

      std::function<void(void)> handleBaseContent = [&]() {
         makeContent();
      };

   public:
      Web_Base(const char* name): Loggable(name) {}

      WebServer* server;
      const char* getPath() { return (const char*)dirPath; }

      void load(const char* path, Serv_Network *_network, WebServer *_server) {
         memcpy(dirPath, path, sizeof(dirPath));
         server = _server;
         network = _network;
         server->on(dirPath, HTTP_GET, handleBaseContent);
      }

      void startPage() {
         // server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
         // server->sendHeader("Pragma", "no-cache");
         // server->sendHeader("Expires", "-1");
         
         //! STRICTLY REQUIRED for correct initialization of string
         buffer[0] = '\0';
         appendStr(HTTP_WEB_START);
      }

      void stopPage() {
         appendStr(HTTP_WEB_END);
         webServer.send(200, "text/html", buffer);
      }

      void startTable(String title) {
         // response->printf("<h1>%s</h1>", title);   
         appendStr("<table style='width:100%'; font-size:60px>");
         String str = "<tr><th colspan='2'>" + title + "</th></tr>";
         appendStr(str.c_str());
      }

      void stopTable() {
         appendStr("</table>");
      }

      void makeNewRow() {
         appendStr("<tr><td>&nbsp;</td></tr>");
      }

      void makeActionForm(const char* path) {
         String str = "<form action='" + String(path) + "' method='post>";
         appendStr(str.c_str());
         makeTextArea("name", "TestName");
         appendStr("<td colspan='2'><input type='submit' value='Submit1'></td>");
         appendStr("</form>");
      }

      void makeTableRow(const char* key, const char* value) {
         appendStr("<tr>");
         String str = "<td>" + String(key) + ":</td>" + 
                           "<td>" + String(value) + "</td>";
         appendStr(str.c_str());
         appendStr("<t/r>");
      }

      void makeTableRow(const char* key, String value) {
         makeTableRow(key, value.c_str());
      }

      void makeTableRow(const char* key, uint64_t value) {
         char val[22];
         sprintf(val, "%lu", value);
         makeTableRow(key, val);
      }
      
      void makeFormButton(String title, String action) {
         makeNewRow();
         String output = "<tr><td colspan='2'><input type='submit' value='" + title +  
               "' formaction='" + action + "' style='display:inline-block; width:100%; height:100px; font-size:40px;'></td></tr>";
         appendStr(output.c_str());
      }

      void makeRow(const char* content) {
         appendStr("<tr>");
         appendStr(content);
         appendStr("</tr>");
      }

      void makeAlertButton(String title) {
         String btnStr = "<td colspan='2'><button onclick='showAlert()' style='display:inline-block; width:100%; height:100px; font-size:40px;'>" 
                              + title + "</button></td>";
         appendStr("<tr>");
         appendStr(btnStr.c_str());
         appendStr("</tr>");
      }

      String buttonStyle = "class='pure-button FontLarge WidthFull Height100px'";

      //! NOTE: button title and href string length is limited
      void makeButton(String title, const char* href, bool isPost = false) {
         makeNewRow();
         String btnStr;

         if (isPost) {
               String isPostString = isPost ? "true" : "false";
               btnStr = "<td colspan='2'><button type='button' onclick=\"sendPostRequest('" + String(href) + 
                     "'," + isPostString + ")\" " + buttonStyle + ">" + title + "</button></td>";
         } else {
               btnStr = "<td colspan='2'><form action='" + String(href) + "'>" + 
                  "<button " + buttonStyle + ">" + title + "</button></form></td>";
         }

         makeRow(btnStr.c_str());
      }

      void makeReturnButton(const char* title = "BACK") {
         makeNewRow();
         String btnStr = "<td colspan='2'><button onclick='goBack()'" + buttonStyle + ">" + String(title) + "</button></td>";
         makeRow(btnStr.c_str());         
      } 

      void makeTextArea(String key, String value) {
         String str = "<tr><td colspan='2'><textarea name='" + key + 
                     "' cols='40' rows='1' maxlength='64' style='width:98%; height:50px; resize:none; font-size:30px;'>" + value + "</textarea></td></tr>";
         appendStr(str.c_str());
      }

      void makeTextField(String key, String value) {
         String str = "<div class='pure-control-group'><td width='30%'>\
         <label for='tf-name' class='Width200px FontLarge'>" + key + "</label></td>\
         <td width='100%'><input type='text' id='tf-name' class='Width95 FontLarge' placeholder='" + key + "' /></td></div>";
         appendStr("<tr>");
         appendStr(str.c_str());
         appendStr("</tr>");
      }
};

char UPLOT_HTML[] = R"!^!(
<!doctype html>
<html>
	<head>
		<meta charset="utf-8">
		<title>Alignment Cost</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">

		<link rel="stylesheet" href="uPlot.min.css">
	</head>
	<body>
		<script src="uPlot.iife.min.js"></script>
		<script>
			function randInt(min, max) {
				return Math.floor(Math.random() * (max - min + 1) + min);
			}

			function genTables(numTables, numSeries, numPoints, gapsAtMod) {
				let tables = [];

				for (let t = 0; t < numTables; t++) {
					let randXvals = new Set();

					while (randXvals.size < numPoints)
						randXvals.add(randInt(0, numPoints * 100));

					randXvals = Array.from(randXvals).sort((a, b) => a - b);

					let table = [
						randXvals
					];

					for (let s = 0; s < numSeries; s++) {
						table.push(randXvals.map(x => {
							let v = randInt(0, 100);
							return v % gapsAtMod == 0 ? null : v;
						}));
					}

					tables.push(table);
				}

				return tables;
			}

			function align(tables) {
				console.time("align");
				let nullModes = tables.map(t => t.map(s => 2));
			//	console.log(nullModes);
				let data = uPlot.join(tables, nullModes);
				console.timeEnd("align");
				return data;
			}

/*
			let tables = [
				[
					[3,5,6,7,20],
					[2,3,4,10,5],
				],
				[
					[1,2,3,4,5,17],
					[7,2,1,null,6,13],
				],
				[
					[9,14,15,16],
					[9,5,null,1],
				]
			];
*/
			let tableCfg = [5, 5, 1000, 5];
			console.log(tableCfg);

			console.time("genTables");
			let tables = genTables.apply(null, tableCfg);
			console.timeEnd("genTables");

//			console.log(tables);

			align(tables);
			align(tables);
			align(tables);
			align(tables);
			align(tables);
			align(tables);

			let data = align(tables);

		//	console.log(data3);

			const opts = {
				width: 2560,
				height: 600,
				scales: {
					x: {
						time: false,
					},
				},
				series: [
					{},
					{
					//	show: false,
						stroke: "red",
						fill: "rgba(255,0,0,0.1)",
						spanGaps: false,
					},
					{
					//	show: false,
						stroke: "green",
						fill: "rgba(0,255,0,0.1)",
						spanGaps: false,
					},
					{
					//	show: false,
						stroke: "blue",
						fill: "rgba(0,0,255,0.1)",
						spanGaps: false,
					},
				],
			};

			console.time("render");
			let u = new uPlot(opts, data, document.body);
			queueMicrotask(() => console.timeEnd("render"));

			setInterval(() => {
				u.series.forEach(s => {
					s.spanGaps = !s.spanGaps;
				});
				u.redraw();
			}, 1000);
		</script>

		<script>
			const xRange = 100;

			const tables3 = [
				[
					null,
					[10,20,10,20,10,20,10,20,10,20,10,20,10,20,10,20,10,20,10,20,10,20,10,20,10,20,10,20,10,20,10,20,10,20,10,20,10,20],
				],
				[
					null,
					[20,10,20,10],
				],
			];

			tables3.forEach(t => {
				t[0] = t[1].map((v, i) => i/t[1].length * xRange);
			});

			const data3 = uPlot.join(tables3);

			let opts3 = {
				width: 1920,
				height: 600,
				scales: {
					x: {
						time: false,
					},
					y: {
						range: [0, 20],
					}
				},
				series: [
					{},
					{
						stroke: "red"
					},
					{
						stroke: "blue",
						fill: "rgba(0,0,255,0.1)",
						paths: uPlot.paths.bars(),
					}
				]
			};

			let u3 = new uPlot(opts3, data3, document.body);
		</script>
	</body>
</html>
)!^!";

char UPLOT_CSS[] = R"!^!(
.uplot, .uplot *, .uplot *::before, .uplot *::after {box-sizing: border-box;}.uplot {font-family: system-ui, -apple-system, "Segoe UI", Roboto, "Helvetica Neue", Arial, "Noto Sans", sans-serif, "Apple Color Emoji", "Segoe UI Emoji", "Segoe UI Symbol", "Noto Color Emoji";line-height: 1.5;width: min-content;}.u-title {text-align: center;font-size: 18px;font-weight: bold;}.u-wrap {position: relative;user-select: none;}.u-over, .u-under {position: absolute;}.u-under {overflow: hidden;}.uplot canvas {display: block;position: relative;width: 100%;height: 100%;}.u-axis {position: absolute;}.u-legend {font-size: 14px;margin: auto;text-align: center;}.u-inline {display: block;}.u-inline * {display: inline-block;}.u-inline tr {margin-right: 16px;}.u-legend th {font-weight: 600;}.u-legend th > * {vertical-align: middle;display: inline-block;}.u-legend .u-marker {width: 1em;height: 1em;margin-right: 4px;background-clip: padding-box !important;}.u-inline.u-live th::after {content: ":";vertical-align: middle;}.u-inline:not(.u-live) .u-value {display: none;}.u-series > * {padding: 4px;}.u-series th {cursor: pointer;}.u-legend .u-off > * {opacity: 0.3;}.u-select {background: rgba(0,0,0,0.07);position: absolute;pointer-events: none;}.u-cursor-x, .u-cursor-y {position: absolute;left: 0;top: 0;pointer-events: none;will-change: transform;}.u-hz .u-cursor-x, .u-vt .u-cursor-y {height: 100%;border-right: 1px dashed #607D8B;}.u-hz .u-cursor-y, .u-vt .u-cursor-x {width: 100%;border-bottom: 1px dashed #607D8B;}.u-cursor-pt {position: absolute;top: 0;left: 0;border-radius: 50%;border: 0 solid;pointer-events: none;will-change: transform;/*this has to be !important since we set inline "background" shorthand */background-clip: padding-box !important;}.u-axis.u-off, .u-select.u-off, .u-cursor-x.u-off, .u-cursor-y.u-off, .u-cursor-pt.u-off {display: none;}
)!^!";

char UPLOT_JS[] = "";

// char UPLOT_JS[] = R"!^!(
// var uPlot=function(){"use strict";const e="u-off",t="u-label",l="width",n="height",i="top",o="bottom",s="left",r="right",u="#000",a=u+"0",f="mousemove",c="mousedown",h="mouseup",d="mouseenter",p="mouseleave",m="dblclick",g="change",x="dppxchange",w="--",_="undefined"!=typeof window,b=_?document:null,v=_?window:null,k=_?navigator:null;let y,M;function S(e,t){if(null!=t){let l=e.classList;!l.contains(t)&&l.add(t)}}function E(e,t){let l=e.classList;l.contains(t)&&l.remove(t)}function z(e,t,l){e.style[t]=l+"px"}function D(e,t,l,n){let i=b.createElement(e);return null!=t&&S(i,t),null!=l&&l.insertBefore(i,n),i}function T(e,t){return D("div",e,t)}const P=new WeakMap;function A(t,l,n,i,o){let s="translate("+l+"px,"+n+"px)";s!=P.get(t)&&(t.style.transform=s,P.set(t,s),0>l||0>n||l>i||n>o?S(t,e):E(t,e))}const W=new WeakMap;function Y(e,t,l){let n=t+l;n!=W.get(e)&&(W.set(e,n),e.style.background=t,e.style.borderColor=l)}const C=new WeakMap;function F(e,t,l,n){let i=t+""+l;i!=C.get(e)&&(C.set(e,i),e.style.height=l+"px",e.style.width=t+"px",e.style.marginLeft=n?-t/2+"px":0,e.style.marginTop=n?-l/2+"px":0)}const H={passive:!0},R={...H,capture:!0};function G(e,t,l,n){t.addEventListener(e,l,n?R:H)}function I(e,t,l,n){t.removeEventListener(e,l,n?R:H)}function L(e,t,l,n){let i;l=l||0;let o=2147483647>=(n=n||t.length-1);for(;n-l>1;)i=o?l+n>>1:le((l+n)/2),e>t[i]?l=i:n=i;return e-t[l]>t[n]-e?n:l}function O(e,t,l,n){for(let i=1==n?t:l;i>=t&&l>=i;i+=n)if(null!=e[i])return i;return-1}function N(e,t,l,n){let i=ue(e),o=ue(t),s=10==l?ae:fe;e==t&&(-1==i?(e*=l,t/=l):(e/=l,t*=l));let r=1==o?ie:le,u=(1==i?le:ie)(s(te(e))),a=r(s(te(t))),f=re(l,u),c=re(l,a);return 10==l&&(0>u&&(f=Se(f,-u)),0>a&&(c=Se(c,-a))),n||2==l?(e=f*i,t=c*o):(e=Me(e,f),t=ye(t,c)),[e,t]}function j(e,t,l,n){let i=N(e,t,l,n);return 0==e&&(i[0]=0),0==t&&(i[1]=0),i}_&&function e(){let t=devicePixelRatio;y!=t&&(y=t,M&&I(g,M,e),M=matchMedia(`(min-resolution: ${y-.001}dppx) and (max-resolution: ${y+.001}dppx)`),G(g,M,e),v.dispatchEvent(new CustomEvent(x)))}();const U=.1,B={mode:3,pad:U},V={pad:0,soft:null,mode:0},J={min:V,max:V};function q(e,t,l,n){return Fe(l)?X(e,t,l):(V.pad=l,V.soft=n?0:null,V.mode=n?3:0,X(e,t,J))}function K(e,t){return null==e?t:e}function X(e,t,l){let n=l.min,i=l.max,o=K(n.pad,0),s=K(i.pad,0),r=K(n.hard,-he),u=K(i.hard,he),a=K(n.soft,he),f=K(i.soft,-he),c=K(n.mode,0),h=K(i.mode,0),d=t-e,p=ae(d),m=se(te(e),te(t)),g=ae(m),x=te(g-p);(1e-9>d||x>10)&&(d=0,0!=e&&0!=t||(d=1e-9,2==c&&a!=he&&(o=0),2==h&&f!=-he&&(s=0)));let w=d||m||1e3,_=ae(w),b=re(10,le(_)),v=Se(Me(e-w*(0==d?0==e?.1:1:o),b/10),9),k=a>e||1!=c&&(3!=c||v>a)&&(2!=c||a>v)?he:a,y=se(r,k>v&&e>=k?k:oe(k,v)),M=Se(ye(t+w*(0==d?0==t?.1:1:s),b/10),9),S=t>f||1!=h&&(3!=h||f>M)&&(2!=h||M>f)?-he:f,E=oe(u,M>S&&S>=t?S:se(S,M));return y==E&&0==y&&(E=100),[y,E]}const Z=new Intl.NumberFormat(_?k.language:"en-US"),$=e=>Z.format(e),Q=Math,ee=Q.PI,te=Q.abs,le=Q.floor,ne=Q.round,ie=Q.ceil,oe=Q.min,se=Q.max,re=Q.pow,ue=Q.sign,ae=Q.log10,fe=Q.log2,ce=(e,t=1)=>Q.asinh(e/t),he=1/0;function de(e){return 1+(0|ae((e^e>>31)-(e>>31)))}function pe(e,t,l){return oe(se(e,t),l)}function me(e){return"function"==typeof e?e:()=>e}const ge=e=>e,xe=(e,t)=>t,we=()=>null,_e=()=>!0,be=(e,t)=>e==t,ve=e=>Se(e,14);function ke(e,t){return ve(Se(ve(e/t))*t)}function ye(e,t){return ve(ie(ve(e/t))*t)}function Me(e,t){return ve(le(ve(e/t))*t)}function Se(e,t=0){if(Ye(e))return e;let l=10**t;return ne(e*l*(1+Number.EPSILON))/l}const Ee=new Map;function ze(e){return((""+e).split(".")[1]||"").length}function De(e,t,l,n){let i=[],o=n.map(ze);for(let s=t;l>s;s++){let t=te(s),l=Se(re(e,s),t);for(let e=0;n.length>e;e++){let r=n[e]*l,u=(0>r||0>s?t:0)+(o[e]>s?o[e]:0),a=Se(r,u);i.push(a),Ee.set(a,u)}}return i}const Te={},Pe=[],Ae=[null,null],We=Array.isArray,Ye=Number.isInteger;function Ce(e){return"string"==typeof e}function Fe(e){let t=!1;if(null!=e){let l=e.constructor;t=null==l||l==Object}return t}function He(e){return null!=e&&"object"==typeof e}const Re=Object.getPrototypeOf(Uint8Array);function Ge(e,t=Fe){let l;if(We(e)){let n=e.find((e=>null!=e));if(We(n)||t(n)){l=Array(e.length);for(let n=0;e.length>n;n++)l[n]=Ge(e[n],t)}else l=e.slice()}else if(e instanceof Re)l=e.slice();else if(t(e)){l={};for(let n in e)l[n]=Ge(e[n],t)}else l=e;return l}function Ie(e){let t=arguments;for(let l=1;t.length>l;l++){let n=t[l];for(let t in n)Fe(e[t])?Ie(e[t],Ge(n[t])):e[t]=Ge(n[t])}return e}function Le(e,t,l){for(let n,i=0,o=-1;t.length>i;i++){let s=t[i];if(s>o){for(n=s-1;n>=0&&null==e[n];)e[n--]=null;for(n=s+1;l>n&&null==e[n];)e[o=n++]=null}}}const Oe="undefined"==typeof queueMicrotask?e=>Promise.resolve().then(e):queueMicrotask,Ne=["January","February","March","April","May","June","July","August","September","October","November","December"],je=["Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"];function Ue(e){return e.slice(0,3)}const Be=je.map(Ue),Ve=Ne.map(Ue),Je={MMMM:Ne,MMM:Ve,WWWW:je,WWW:Be};function qe(e){return(10>e?"0":"")+e}const Ke={YYYY:e=>e.getFullYear(),YY:e=>(e.getFullYear()+"").slice(2),MMMM:(e,t)=>t.MMMM[e.getMonth()],MMM:(e,t)=>t.MMM[e.getMonth()],MM:e=>qe(e.getMonth()+1),M:e=>e.getMonth()+1,DD:e=>qe(e.getDate()),D:e=>e.getDate(),WWWW:(e,t)=>t.WWWW[e.getDay()],WWW:(e,t)=>t.WWW[e.getDay()],HH:e=>qe(e.getHours()),H:e=>e.getHours(),h:e=>{let t=e.getHours();return 0==t?12:t>12?t-12:t},AA:e=>12>e.getHours()?"AM":"PM",aa:e=>12>e.getHours()?"am":"pm",a:e=>12>e.getHours()?"a":"p",mm:e=>qe(e.getMinutes()),m:e=>e.getMinutes(),ss:e=>qe(e.getSeconds()),s:e=>e.getSeconds(),fff:e=>function(e){return(10>e?"00":100>e?"0":"")+e}(e.getMilliseconds())};function Xe(e,t){t=t||Je;let l,n=[],i=/\{([a-z]+)\}|[^{]+/gi;for(;l=i.exec(e);)n.push("{"==l[0][0]?Ke[l[1]]:l[0]);return e=>{let l="";for(let i=0;n.length>i;i++)l+="string"==typeof n[i]?n[i]:n[i](e,t);return l}}const Ze=(new Intl.DateTimeFormat).resolvedOptions().timeZone,$e=e=>e%1==0,Qe=[1,2,2.5,5],et=De(10,-16,0,Qe),tt=De(10,0,16,Qe),lt=tt.filter($e),nt=et.concat(tt),it="{YYYY}",ot="\n"+it,st="{M}/{D}",rt="\n"+st,ut=rt+"/{YY}",at="{aa}",ft="{h}:{mm}"+at,ct="\n"+ft,ht=":{ss}",dt=null;function pt(e){let t=1e3*e,l=60*t,n=60*l,i=24*n,o=30*i,s=365*i;return[(1==e?De(10,0,3,Qe).filter($e):De(10,-3,0,Qe)).concat([t,5*t,10*t,15*t,30*t,l,5*l,10*l,15*l,30*l,n,2*n,3*n,4*n,6*n,8*n,12*n,i,2*i,3*i,4*i,5*i,6*i,7*i,8*i,9*i,10*i,15*i,o,2*o,3*o,4*o,6*o,s,2*s,5*s,10*s,25*s,50*s,100*s]),[[s,it,dt,dt,dt,dt,dt,dt,1],[28*i,"{MMM}",ot,dt,dt,dt,dt,dt,1],[i,st,ot,dt,dt,dt,dt,dt,1],[n,"{h}"+at,ut,dt,rt,dt,dt,dt,1],[l,ft,ut,dt,rt,dt,dt,dt,1],[t,ht,ut+" "+ft,dt,rt+" "+ft,dt,ct,dt,1],[e,ht+".{fff}",ut+" "+ft,dt,rt+" "+ft,dt,ct,dt,1]],function(t){return(r,u,a,f,c,h)=>{let d=[],p=c>=s,m=c>=o&&s>c,g=t(a),x=Se(g*e,3),w=yt(g.getFullYear(),p?0:g.getMonth(),m||p?1:g.getDate()),_=Se(w*e,3);if(m||p){let l=m?c/o:0,n=p?c/s:0,i=x==_?x:Se(yt(w.getFullYear()+n,w.getMonth()+l,1)*e,3),r=new Date(ne(i/e)),u=r.getFullYear(),a=r.getMonth();for(let o=0;f>=i;o++){let s=yt(u+n*o,a+l*o,1),r=s-t(Se(s*e,3));i=Se((+s+r)*e,3),i>f||d.push(i)}}else{let o=i>c?c:i,s=_+(le(a)-le(x))+ye(x-_,o);d.push(s);let p=t(s),m=p.getHours()+p.getMinutes()/l+p.getSeconds()/n,g=c/n,w=h/r.axes[u]._space;for(;s=Se(s+c,1==e?0:3),f>=s;)if(g>1){let e=le(Se(m+g,6))%24,l=t(s).getHours()-e;l>1&&(l=-1),s-=l*n,m=(m+g)%24,.7>Se((s-d[d.length-1])/c,3)*w||d.push(s)}else d.push(s)}return d}}]}const[mt,gt,xt]=pt(1),[wt,_t,bt]=pt(.001);function vt(e,t){return e.map((e=>e.map(((l,n)=>0==n||8==n||null==l?l:t(1==n||0==e[8]?l:e[1]+l)))))}function kt(e,t){return(l,n,i,o,s)=>{let r,u,a,f,c,h,d=t.find((e=>s>=e[0]))||t[t.length-1];return n.map((t=>{let l=e(t),n=l.getFullYear(),i=l.getMonth(),o=l.getDate(),s=l.getHours(),p=l.getMinutes(),m=l.getSeconds(),g=n!=r&&d[2]||i!=u&&d[3]||o!=a&&d[4]||s!=f&&d[5]||p!=c&&d[6]||m!=h&&d[7]||d[1];return r=n,u=i,a=o,f=s,c=p,h=m,g(l)}))}}function yt(e,t,l){return new Date(e,t,l)}function Mt(e,t){return t(e)}function St(e,t){return(l,n,i,o)=>null==o?w:t(e(n))}De(2,-53,53,[1]);const Et={show:!0,live:!0,isolate:!1,mount:()=>{},markers:{show:!0,width:2,stroke:function(e,t){let l=e.series[t];return l.width?l.stroke(e,t):l.points.width?l.points.stroke(e,t):null},fill:function(e,t){return e.series[t].fill(e,t)},dash:"solid"},idx:null,idxs:null,values:[]},zt=[0,0];function Dt(e,t,l,n=!0){return e=>{0==e.button&&(!n||e.target==t)&&l(e)}}function Tt(e,t,l,n=!0){return e=>{(!n||e.target==t)&&l(e)}}const Pt={show:!0,x:!0,y:!0,lock:!1,move:function(e,t,l){return zt[0]=t,zt[1]=l,zt},points:{show:function(e,t){let i=e.cursor.points,o=T(),s=i.size(e,t);z(o,l,s),z(o,n,s);let r=s/-2;z(o,"marginLeft",r),z(o,"marginTop",r);let u=i.width(e,t,s);return u&&z(o,"borderWidth",u),o},size:function(e,t){return e.series[t].points.size},width:0,stroke:function(e,t){let l=e.series[t].points;return l._stroke||l._fill},fill:function(e,t){let l=e.series[t].points;return l._fill||l._stroke}},bind:{mousedown:Dt,mouseup:Dt,click:Dt,dblclick:Dt,mousemove:Tt,mouseleave:Tt,mouseenter:Tt},drag:{setScale:!0,x:!0,y:!1,dist:0,uni:null,click:(e,t)=>{t.stopPropagation(),t.stopImmediatePropagation()},_x:!1,_y:!1},focus:{prox:-1,bias:0},left:-10,top:-10,idx:null,dataIdx:function(e,t,l){return l},idxs:null,event:null},At={show:!0,stroke:"rgba(0,0,0,0.07)",width:2},Wt=Ie({},At,{filter:xe}),Yt=Ie({},Wt,{size:10}),Ct=Ie({},At,{show:!1}),Ft='12px system-ui, -apple-system, "Segoe UI", Roboto, "Helvetica Neue", Arial, "Noto Sans", sans-serif, "Apple Color Emoji", "Segoe UI Emoji", "Segoe UI Symbol", "Noto Color Emoji"',Ht="bold "+Ft,Rt={show:!0,scale:"x",stroke:u,space:50,gap:5,size:50,labelGap:0,labelSize:30,labelFont:Ht,side:2,grid:Wt,ticks:Yt,border:Ct,font:Ft,lineGap:1.5,rotate:0},Gt={show:!0,scale:"x",auto:!1,sorted:1,min:he,max:-he,idxs:[]};function It(e,t){return t.map((e=>null==e?"":$(e)))}function Lt(e,t,l,n,i,o,s){let r=[],u=Ee.get(i)||0;for(let e=l=s?l:Se(ye(l,i),u);n>=e;e=Se(e+i,u))r.push(Object.is(e,-0)?0:e);return r}function Ot(e,t,l,n,i){const o=[],s=e.scales[e.axes[t].scale].log,r=le((10==s?ae:fe)(l));i=re(s,r),10==s&&0>r&&(i=Se(i,-r));let u=l;do{o.push(u),u+=i,10==s&&(u=Se(u,Ee.get(i))),i*s>u||(i=u)}while(n>=u);return o}function Nt(e,t,l,n,i){let o=e.scales[e.axes[t].scale].asinh,s=n>o?Ot(e,t,se(o,l),n,i):[o],r=0>n||l>0?[]:[0];return(-o>l?Ot(e,t,se(o,-n),-l,i):[o]).reverse().map((e=>-e)).concat(r,s)}const jt=/./,Ut=/[12357]/,Bt=/[125]/,Vt=/1/,Jt=(e,t,l,n)=>e.map(((e,i)=>4==t&&0==e||i%n==0&&l.test(e.toExponential()[0>e?1:0])?e:null));function qt(e,t,l){let n=e.axes[l],i=n.scale,o=e.scales[i],s=e.valToPos,r=n._space,u=s(10,i),a=s(9,i)-u<r?s(7,i)-u<r?s(5,i)-u<r?Vt:Bt:Ut:jt;if(a==Vt){let e=te(s(1,i)-u);if(r>e)return Jt(t.slice().reverse(),o.distr,a,ie(r/e)).reverse()}return Jt(t,o.distr,a,1)}function Kt(e,t,l){let n=e.axes[l],i=n.scale,o=n._space,s=e.valToPos,r=te(s(1,i)-s(2,i));return o>r?Jt(t.slice().reverse(),3,jt,ie(o/r)).reverse():t}function Xt(e,t,l,n){return null==n?w:null==t?"":$(t)}const Zt={show:!0,scale:"y",stroke:u,space:30,gap:5,size:50,labelGap:0,labelSize:30,labelFont:Ht,side:3,grid:Wt,ticks:Yt,border:Ct,font:Ft,lineGap:1.5,rotate:0},$t={scale:null,auto:!0,sorted:0,min:he,max:-he},Qt=(e,t,l,n,i)=>i,el={show:!0,auto:!0,sorted:0,gaps:Qt,alpha:1,facets:[Ie({},$t,{scale:"x"}),Ie({},$t,{scale:"y"})]},tl={scale:"y",auto:!0,sorted:0,show:!0,spanGaps:!1,gaps:Qt,alpha:1,points:{show:function(e,t){let{scale:l,idxs:n}=e.series[0],i=e._data[0],o=e.valToPos(i[n[0]],l,!0),s=e.valToPos(i[n[1]],l,!0);return te(s-o)/(e.series[t].points.space*y)>=n[1]-n[0]},filter:null},values:null,min:he,max:-he,idxs:[],path:null,clip:null};function ll(e,t,l){return l/10}const nl={time:!0,auto:!0,distr:1,log:10,asinh:1,min:null,max:null,dir:1,ori:0},il=Ie({},nl,{time:!1,ori:1}),ol={};function sl(e){let t=ol[e];return t||(t={key:e,plots:[],sub(e){t.plots.push(e)},unsub(e){t.plots=t.plots.filter((t=>t!=e))},pub(e,l,n,i,o,s,r){for(let u=0;t.plots.length>u;u++)t.plots[u]!=l&&t.plots[u].pub(e,l,n,i,o,s,r)}},null!=e&&(ol[e]=t)),t}function rl(e,t,l){const n=e.mode,i=e.series[t],o=2==n?e._data[t]:e._data,s=e.scales,r=e.bbox;let u=o[0],a=2==n?o[1]:o[t],f=2==n?s[i.facets[0].scale]:s[e.series[0].scale],c=2==n?s[i.facets[1].scale]:s[i.scale],h=r.left,d=r.top,p=r.width,m=r.height,g=e.valToPosH,x=e.valToPosV;return 0==f.ori?l(i,u,a,f,c,g,x,h,d,p,m,ml,xl,_l,vl,yl):l(i,u,a,f,c,x,g,d,h,m,p,gl,wl,bl,kl,Ml)}function ul(e,t){let l=0,n=0,i=K(e.bands,Pe);for(let e=0;i.length>e;e++){let o=i[e];o.series[0]==t?l=o.dir:o.series[1]==t&&(n|=1==o.dir?1:2)}return[l,1==n?-1:2==n?1:3==n?2:0]}function al(e,t,l,n,i){let o=e.series[t],s=e.scales[2==e.mode?o.facets[1].scale:o.scale];return-1==i?s.min:1==i?s.max:3==s.distr?1==s.dir?s.min:s.max:0}function fl(e,t,l,n,i,o){return rl(e,t,((e,t,s,r,u,a,f,c,h,d,p)=>{let m=e.pxRound;const g=0==r.ori?xl:wl;let x,w;1==r.dir*(0==r.ori?1:-1)?(x=l,w=n):(x=n,w=l);let _=m(a(t[x],r,d,c)),b=m(f(s[x],u,p,h)),v=m(a(t[w],r,d,c)),k=m(f(1==o?u.max:u.min,u,p,h)),y=new Path2D(i);return g(y,v,k),g(y,_,k),g(y,_,b),y}))}function cl(e,t,l,n,i,o){let s=null;if(e.length>0){s=new Path2D;const r=0==t?_l:bl;let u=l;for(let t=0;e.length>t;t++){let l=e[t];if(l[1]>l[0]){let e=l[0]-u;e>0&&r(s,u,n,e,n+o),u=l[1]}}let a=l+i-u,f=10;a>0&&r(s,u,n-f/2,a,n+o+f)}return s}function hl(e,t,l,n,i,o,s){let r=[],u=e.length;for(let a=1==i?l:n;a>=l&&n>=a;a+=i)if(null===t[a]){let f=a,c=a;if(1==i)for(;++a<=n&&null===t[a];)c=a;else for(;--a>=l&&null===t[a];)c=a;let h=o(e[f]),d=c==f?h:o(e[c]),p=f-i;h=s>0||0>p||p>=u?h:o(e[p]);let m=c+i;d=0>s||0>m||m>=u?d:o(e[m]),h>d||r.push([h,d])}return r}function dl(e){return 0==e?ge:1==e?ne:t=>ke(t,e)}function pl(e){let t=0==e?ml:gl,l=0==e?(e,t,l,n,i,o)=>{e.arcTo(t,l,n,i,o)}:(e,t,l,n,i,o)=>{e.arcTo(l,t,i,n,o)},n=0==e?(e,t,l,n,i)=>{e.rect(t,l,n,i)}:(e,t,l,n,i)=>{e.rect(l,t,i,n)};return(e,i,o,s,r,u=0,a=0)=>{0==u&&0==a?n(e,i,o,s,r):(u=oe(u,s/2,r/2),a=oe(a,s/2,r/2),t(e,i+u,o),l(e,i+s,o,i+s,o+r,u),l(e,i+s,o+r,i,o+r,a),l(e,i,o+r,i,o,a),l(e,i,o,i+s,o,u),e.closePath())}}const ml=(e,t,l)=>{e.moveTo(t,l)},gl=(e,t,l)=>{e.moveTo(l,t)},xl=(e,t,l)=>{e.lineTo(t,l)},wl=(e,t,l)=>{e.lineTo(l,t)},_l=pl(0),bl=pl(1),vl=(e,t,l,n,i,o)=>{e.arc(t,l,n,i,o)},kl=(e,t,l,n,i,o)=>{e.arc(l,t,n,i,o)},yl=(e,t,l,n,i,o,s)=>{e.bezierCurveTo(t,l,n,i,o,s)},Ml=(e,t,l,n,i,o,s)=>{e.bezierCurveTo(l,t,i,n,s,o)};function Sl(){return(e,t,l,n,i)=>rl(e,t,((t,o,s,r,u,a,f,c,h,d,p)=>{let m,g,{pxRound:x,points:w}=t;0==r.ori?(m=ml,g=vl):(m=gl,g=kl);const _=Se(w.width*y,3);let b=(w.size-w.width)/2*y,v=Se(2*b,3),k=new Path2D,M=new Path2D,{left:S,top:E,width:z,height:D}=e.bbox;_l(M,S-v,E-v,z+2*v,D+2*v);const T=e=>{if(null!=s[e]){let t=x(a(o[e],r,d,c)),l=x(f(s[e],u,p,h));m(k,t+b,l),g(k,t,l,b,0,2*ee)}};if(i)i.forEach(T);else for(let e=l;n>=e;e++)T(e);return{stroke:_>0?k:null,fill:k,clip:M,flags:3}}))}function El(e){return(t,l,n,i,o,s)=>{n!=i&&(o!=n&&s!=n&&e(t,l,n),o!=i&&s!=i&&e(t,l,i),e(t,l,s))}}const zl=El(xl),Dl=El(wl);function Tl(e){const t=K(e?.alignGaps,0);return(e,l,n,i)=>rl(e,l,((o,s,r,u,a,f,c,h,d,p,m)=>{let g,x,w=o.pxRound,_=e=>w(f(e,u,p,h)),b=e=>w(c(e,a,m,d));0==u.ori?(g=xl,x=zl):(g=wl,x=Dl);const v=u.dir*(0==u.ori?1:-1),k={stroke:new Path2D,fill:null,clip:null,band:null,gaps:null,flags:1},y=k.stroke;let M,S,E,z=he,D=-he,T=_(s[1==v?n:i]),P=O(r,n,i,1*v),A=O(r,n,i,-1*v),W=_(s[P]),Y=_(s[A]),C=!1;for(let e=1==v?n:i;e>=n&&i>=e;e+=v){let t=_(s[e]),l=r[e];t==T?null!=l?(S=b(l),z==he&&(g(y,t,S),M=S),z=oe(S,z),D=se(S,D)):null===l&&(C=!0):(z!=he&&(x(y,T,z,D,M,S),E=T),null!=l?(S=b(l),g(y,t,S),z=D=M=S):(z=he,D=-he,null===l&&(C=!0)),T=t)}z!=he&&z!=D&&E!=T&&x(y,T,z,D,M,S);let[F,H]=ul(e,l);if(null!=o.fill||0!=F){let t=k.fill=new Path2D(y),n=b(o.fillTo(e,l,o.min,o.max,F));g(t,Y,n),g(t,W,n)}if(!o.spanGaps){let a=[];C&&a.push(...hl(s,r,n,i,v,_,t)),k.gaps=a=o.gaps(e,l,n,i,a),k.clip=cl(a,u.ori,h,d,p,m)}return 0!=H&&(k.band=2==H?[fl(e,l,n,i,y,-1),fl(e,l,n,i,y,1)]:fl(e,l,n,i,y,H)),k}))}function Pl(e,t,l,n,i){const o=e.length;if(2>o)return null;const s=new Path2D;if(l(s,e[0],t[0]),2==o)n(s,e[1],t[1]);else{let l=Array(o),n=Array(o-1),r=Array(o-1),u=Array(o-1);for(let l=0;o-1>l;l++)r[l]=t[l+1]-t[l],u[l]=e[l+1]-e[l],n[l]=r[l]/u[l];l[0]=n[0];for(let e=1;o-1>e;e++)0===n[e]||0===n[e-1]||n[e-1]>0!=n[e]>0?l[e]=0:(l[e]=3*(u[e-1]+u[e])/((2*u[e]+u[e-1])/n[e-1]+(u[e]+2*u[e-1])/n[e]),isFinite(l[e])||(l[e]=0));l[o-1]=n[o-2];for(let n=0;o-1>n;n++)i(s,e[n]+u[n]/3,t[n]+l[n]*u[n]/3,e[n+1]-u[n]/3,t[n+1]-l[n+1]*u[n]/3,e[n+1],t[n+1])}return s}const Al=new Set;function Wl(){for(let e of Al)e.syncRect(!0)}_&&(G("resize",v,Wl),G("scroll",v,Wl,!0),G(x,v,(()=>{Jl.pxRatio=y})));const Yl=Tl(),Cl=Sl();function Fl(e,t,l,n){return(n?[e[0],e[1]].concat(e.slice(2)):[e[0]].concat(e.slice(1))).map(((e,n)=>Hl(e,n,t,l)))}function Hl(e,t,l,n){return Ie({},0==t?l:n,e)}function Rl(e,t,l){return null==t?Ae:[t,l]}const Gl=Rl;function Il(e,t,l){return null==t?Ae:q(t,l,U,!0)}function Ll(e,t,l,n){return null==t?Ae:N(t,l,e.scales[n].log,!1)}const Ol=Ll;function Nl(e,t,l,n){return null==t?Ae:j(t,l,e.scales[n].log,!1)}const jl=Nl;function Ul(e,t,l,n,i){let o=se(de(e),de(t)),s=t-e,r=L(i/n*s,l);do{let e=l[r],t=n*e/s;if(t>=i&&17>=o+(5>e?Ee.get(e):0))return[e,t]}while(++r<l.length);return[0,0]}function Bl(e){let t,l;return[e=e.replace(/(\d+)px/,((e,n)=>(t=ne((l=+n)*y))+"px")),t,l]}function Vl(e){e.show&&[e.font,e.labelFont].forEach((e=>{let t=Se(e[2]*y,1);e[0]=e[0].replace(/[0-9.]+px/,t+"px"),e[1]=t}))}function Jl(u,g,_){const k={mode:K(u.mode,1)},M=k.mode;function P(e,t){return((3==t.distr?ae(e>0?e:t.clamp(k,e,t.min,t.max,t.key)):4==t.distr?ce(e,t.asinh):e)-t._min)/(t._max-t._min)}function W(e,t,l,n){let i=P(e,t);return n+l*(-1==t.dir?1-i:i)}function C(e,t,l,n){let i=P(e,t);return n+l*(-1==t.dir?i:1-i)}function H(e,t,l,n){return 0==t.ori?W(e,t,l,n):C(e,t,l,n)}k.valToPosH=W,k.valToPosV=C;let R=!1;k.status=0;const O=k.root=T("uplot");null!=u.id&&(O.id=u.id),S(O,u.class),u.title&&(T("u-title",O).textContent=u.title);const V=D("canvas"),J=k.ctx=V.getContext("2d"),X=T("u-wrap",O);G("click",X,(e=>{e.target===$&&(Dn!=Mn||Tn!=Sn)&&Fn.click(k,e)}),!0);const Z=k.under=T("u-under",X);X.appendChild(V);const $=k.over=T("u-over",X),le=+K((u=Ge(u)).pxAlign,1),ue=dl(le);(u.plugins||[]).forEach((e=>{e.opts&&(u=e.opts(k,u)||u)}));const fe=u.ms||.001,de=k.series=1==M?Fl(u.series||[],Gt,tl,!1):function(e,t){return e.map(((e,l)=>0==l?null:Ie({},t,e)))}(u.series||[null],el),ge=k.axes=Fl(u.axes||[],Rt,Zt,!0),ve=k.scales={},Me=k.bands=u.bands||[];Me.forEach((e=>{e.fill=me(e.fill||null),e.dir=K(e.dir,-1)}));const De=2==M?de[1].facets[0].scale:de[0].scale,Ye={axes:function(){for(let e=0;ge.length>e;e++){let t=ge[e];if(!t.show||!t._show)continue;let l,n,u=t.side,a=u%2,f=t.stroke(k,e),c=0==u||3==u?-1:1;if(t.label){let e=ne((t._lpos+t.labelGap*c)*y);sn(t.labelFont[0],f,"center",2==u?i:o),J.save(),1==a?(l=n=0,J.translate(e,ne(Ut+Vt/2)),J.rotate((3==u?-ee:ee)/2)):(l=ne(jt+Bt/2),n=e),J.fillText(t.label,l,n),J.restore()}let[h,d]=t._found;if(0==d)continue;let p=ve[t.scale],m=0==a?Bt:Vt,g=0==a?jt:Ut,x=ne(t.gap*y),w=t._splits,_=2==p.distr?w.map((e=>en[e])):w,b=2==p.distr?en[w[1]]-en[w[0]]:h,v=t.ticks,M=t.border,S=v.show?ne(v.size*y):0,E=t._rotate*-ee/180,z=ue(t._pos*y),D=z+(S+x)*c;n=0==a?D:0,l=1==a?D:0,sn(t.font[0],f,1==t.align?s:2==t.align?r:E>0?s:0>E?r:0==a?"center":3==u?r:s,E||1==a?"middle":2==u?i:o);let T=t.font[1]*t.lineGap,P=w.map((e=>ue(H(e,p,m,g)))),A=t._values;for(let e=0;A.length>e;e++){let t=A[e];if(null!=t){0==a?l=P[e]:n=P[e],t=""+t;let i=-1==t.indexOf("\n")?[t]:t.split(/\n/gm);for(let e=0;i.length>e;e++){let t=i[e];E?(J.save(),J.translate(l,n+e*T),J.rotate(E),J.fillText(t,0,0),J.restore()):J.fillText(t,l,n+e*T)}}}v.show&&pn(P,v.filter(k,_,e,d,b),a,u,z,S,Se(v.width*y,3),v.stroke(k,e),v.dash,v.cap);let W=t.grid;W.show&&pn(P,W.filter(k,_,e,d,b),a,0==a?2:1,0==a?Ut:jt,0==a?Vt:Bt,Se(W.width*y,3),W.stroke(k,e),W.dash,W.cap),M.show&&pn([z],[1],0==a?1:0,0==a?1:2,1==a?Ut:jt,1==a?Vt:Bt,Se(M.width*y,3),M.stroke(k,e),M.dash,M.cap)}xi("drawAxes")},series:function(){Sl>0&&(de.forEach(((e,t)=>{if(t>0&&e.show&&(un(t,!1),un(t,!0),null==e._paths)){let l=2==M?[0,g[t][0].length-1]:function(e){let t=pe(El-1,0,Sl-1),l=pe(zl+1,0,Sl-1);for(;null==e[t]&&t>0;)t--;for(;null==e[l]&&Sl-1>l;)l++;return[t,l]}(g[t]);e._paths=e.paths(k,t,l[0],l[1])}})),de.forEach(((e,t)=>{if(t>0&&e.show){Ql!=e.alpha&&(J.globalAlpha=Ql=e.alpha),null!=e._paths&&an(t,!1);{let l=null!=e._paths?e._paths.gaps:null,n=e.points.show(k,t,El,zl,l),i=e.points.filter(k,t,n,l);(n||i)&&(e.points._paths=e.points.paths(k,t,El,zl,i),an(t,!0))}1!=Ql&&(J.globalAlpha=Ql=1),xi("drawSeries",t)}})))}},Re=(u.drawOrder||["axes","series"]).map((e=>Ye[e]));function Le(e){let t=ve[e];if(null==t){let l=(u.scales||Te)[e]||Te;if(null!=l.from)Le(l.from),ve[e]=Ie({},ve[l.from],l,{key:e});else{t=ve[e]=Ie({},e==De?nl:il,l),t.key=e;let n=t.time,i=t.range,o=We(i);if((e!=De||2==M&&!n)&&(!o||null!=i[0]&&null!=i[1]||(i={min:null==i[0]?B:{mode:1,hard:i[0],soft:i[0]},max:null==i[1]?B:{mode:1,hard:i[1],soft:i[1]}},o=!1),!o&&Fe(i))){let e=i;i=(t,l,n)=>null==l?Ae:q(l,n,e)}t.range=me(i||(n?Gl:e==De?3==t.distr?Ol:4==t.distr?jl:Rl:3==t.distr?Ll:4==t.distr?Nl:Il)),t.auto=me(!o&&t.auto),t.clamp=me(t.clamp||ll),t._min=t._max=null}}}Le("x"),Le("y"),1==M&&de.forEach((e=>{Le(e.scale)})),ge.forEach((e=>{Le(e.scale)}));for(let e in u.scales)Le(e);const Ne=ve[De],je=Ne.distr;let Ue,Be;0==Ne.ori?(S(O,"u-hz"),Ue=W,Be=C):(S(O,"u-vt"),Ue=C,Be=W);const Ve={};for(let e in ve){let t=ve[e];null==t.min&&null==t.max||(Ve[e]={min:t.min,max:t.max},t.min=t.max=null)}const Je=u.tzDate||(e=>new Date(ne(e/fe))),qe=u.fmtDate||Xe,Ke=1==fe?xt(Je):bt(Je),Ze=kt(Je,vt(1==fe?gt:_t,qe)),$e=St(Je,Mt("{YYYY}-{MM}-{DD} {h}:{mm}{aa}",qe)),Qe=[],et=k.legend=Ie({},Et,u.legend),tt=et.show,it=et.markers;let ot,st,rt;et.idxs=Qe,it.width=me(it.width),it.dash=me(it.dash),it.stroke=me(it.stroke),it.fill=me(it.fill);let ut,at=[],ft=[],ct=!1,ht={};if(et.live){const e=de[1]?de[1].values:null;ct=null!=e,ut=ct?e(k,1,0):{_:0};for(let e in ut)ht[e]=w}if(tt)if(ot=D("table","u-legend",O),rt=D("tbody",null,ot),et.mount(k,ot),ct){st=D("thead",null,ot,rt);let e=D("tr",null,st);for(var dt in D("th",null,e),ut)D("th",t,e).textContent=dt}else S(ot,"u-inline"),et.live&&S(ot,"u-live");const pt={show:!0},yt={show:!1},zt=new Map;function Dt(e,t,l,n=!0){const i=zt.get(t)||{},o=pl.bind[e](k,t,l,n);o&&(G(e,t,i[e]=o),zt.set(t,i))}function Tt(e,t){const l=zt.get(t)||{};for(let n in l)null!=e&&n!=e||(I(n,t,l[n]),delete l[n]);null==e&&zt.delete(t)}let At=0,Wt=0,Yt=0,Ct=0,Ft=0,Ht=0,jt=0,Ut=0,Bt=0,Vt=0;k.bbox={};let Jt=!1,$t=!1,Qt=!1,ol=!1,rl=!1,ul=!1;function fl(e,t,l){(l||e!=k.width||t!=k.height)&&cl(e,t),xn(!1),Qt=!0,$t=!0,0>pl.left||(ol=ul=!0),An()}function cl(e,t){k.width=At=Yt=e,k.height=Wt=Ct=t,Ft=Ht=0,function(){let e=!1,t=!1,l=!1,n=!1;ge.forEach((i=>{if(i.show&&i._show){let{side:o,_size:s}=i,r=s+(null!=i.label?i.labelSize:0);r>0&&(o%2?(Yt-=r,3==o?(Ft+=r,n=!0):l=!0):(Ct-=r,0==o?(Ht+=r,e=!0):t=!0))}})),vl[0]=e,vl[1]=l,vl[2]=t,vl[3]=n,Yt-=Ml[1]+Ml[3],Ft+=Ml[3],Ct-=Ml[2]+Ml[0],Ht+=Ml[0]}(),function(){let e=Ft+Yt,t=Ht+Ct,l=Ft,n=Ht;function i(i,o){switch(i){case 1:return e+=o,e-o;case 2:return t+=o,t-o;case 3:return l-=o,l+o;case 0:return n-=o,n+o}}ge.forEach((e=>{if(e.show&&e._show){let t=e.side;e._pos=i(t,e._size),null!=e.label&&(e._lpos=i(t,e.labelSize))}}))}();let l=k.bbox;jt=l.left=ke(Ft*y,.5),Ut=l.top=ke(Ht*y,.5),Bt=l.width=ke(Yt*y,.5),Vt=l.height=ke(Ct*y,.5)}const hl=3;k.setSize=function({width:e,height:t}){fl(e,t)};const pl=k.cursor=Ie({},Pt,{drag:{y:2==M}},u.cursor),ml=e=>{pl.event=e};pl.idxs=Qe,pl._lock=!1;let gl=pl.points;gl.show=me(gl.show),gl.size=me(gl.size),gl.stroke=me(gl.stroke),gl.width=me(gl.width),gl.fill=me(gl.fill);const xl=k.focus=Ie({},u.focus||{alpha:.3},pl.focus),wl=xl.prox>=0;let _l=[null];function bl(l,n){if(1==M||n>0){let e=1==M&&ve[l.scale].time,t=l.value;l.value=e?Ce(t)?St(Je,Mt(t,qe)):t||$e:t||Xt,l.label=l.label||(e?"Time":"Value")}if(n>0){l.width=null==l.width?1:l.width,l.paths=l.paths||Yl||we,l.fillTo=me(l.fillTo||al),l.pxAlign=+K(l.pxAlign,le),l.pxRound=dl(l.pxAlign),l.stroke=me(l.stroke||null),l.fill=me(l.fill||null),l._stroke=l._fill=l._paths=l._focus=null;let e=function(e){return Se(1*(3+2*(e||1)),3)}(se(1,l.width)),t=l.points=Ie({},{size:e,width:se(1,.2*e),stroke:l.stroke,space:2*e,paths:Cl,_stroke:null,_fill:null},l.points);t.show=me(t.show),t.filter=me(t.filter),t.fill=me(t.fill),t.stroke=me(t.stroke),t.paths=me(t.paths),t.pxAlign=l.pxAlign}if(tt){let i=function(l,n){if(0==n&&(ct||!et.live||2==M))return Ae;let i=[],o=D("tr","u-series",rt,rt.childNodes[n]);S(o,l.class),l.show||S(o,e);let s=D("th",null,o);if(it.show){let e=T("u-marker",s);if(n>0){let t=it.width(k,n);t&&(e.style.border=t+"px "+it.dash(k,n)+" "+it.stroke(k,n)),e.style.background=it.fill(k,n)}}let r=T(t,s);for(var u in r.textContent=l.label,n>0&&(it.show||(r.style.color=l.width>0?it.stroke(k,n):it.fill(k,n)),Dt("click",s,(e=>{if(pl._lock)return;ml(e);let t=de.indexOf(l);if((e.ctrlKey||e.metaKey)!=et.isolate){let e=de.some(((e,l)=>l>0&&l!=t&&e.show));de.forEach(((l,n)=>{n>0&&Nn(n,e?n==t?pt:yt:pt,!0,_i.setSeries)}))}else Nn(t,{show:!l.show},!0,_i.setSeries)}),!1),wl&&Dt(d,s,(e=>{pl._lock||(ml(e),Nn(de.indexOf(l),Vn,!0,_i.setSeries))}),!1)),ut){let e=D("td","u-value",o);e.textContent="--",i.push(e)}return[o,i]}(l,n);at.splice(n,0,i[0]),ft.splice(n,0,i[1]),et.values.push(null)}if(pl.show){Qe.splice(n,0,null);let e=function(e,t){if(t>0){let l=pl.points.show(k,t);if(l)return S(l,"u-cursor-pt"),S(l,e.class),A(l,-10,-10,Yt,Ct),$.insertBefore(l,_l[t]),l}}(l,n);e&&_l.splice(n,0,e)}xi("addSeries",n)}k.addSeries=function(e,t){t=null==t?de.length:t,e=1==M?Hl(e,t,Gt,tl):Hl(e,t,null,el),de.splice(t,0,e),bl(de[t],t)},k.delSeries=function(e){if(de.splice(e,1),tt){et.values.splice(e,1),ft.splice(e,1);let t=at.splice(e,1)[0];Tt(null,t.firstChild),t.remove()}pl.show&&(Qe.splice(e,1),_l.length>1&&_l.splice(e,1)[0].remove()),xi("delSeries",e)};const vl=[!1,!1,!1,!1];function kl(e,t,l){let[n,i,o,s]=l,r=t%2,u=0;return 0==r&&(s||i)&&(u=0==t&&!n||2==t&&!o?ne(Rt.size/3):0),1==r&&(n||o)&&(u=1==t&&!i||3==t&&!s?ne(Zt.size/2):0),u}const yl=k.padding=(u.padding||[kl,kl,kl,kl]).map((e=>me(K(e,kl)))),Ml=k._padding=yl.map(((e,t)=>e(k,t,vl,0)));let Sl,El=null,zl=null;const Dl=1==M?de[0].idxs:null;let Tl,Pl,Wl,Jl,ql,Kl,Xl,Zl,$l,Ql,en=null,tn=!1;function ln(e,t){if(g=null==e?[]:e,2==M){Sl=0;for(let e=1;de.length>e;e++)Sl+=g[e][0].length;k._data=g}else{0==g.length&&(g=[[]]),en=g[0],Sl=en.length;let e=g;if(2==je){e=g.slice();let t=e[0]=Array(Sl);for(let e=0;Sl>e;e++)t[e]=e}k._data=g=e}if(k.data=g,xn(!0),xi("setData"),2==je&&(Qt=!0),!1!==t){let e=Ne;e.auto(k,tn)?nn():On(De,e.min,e.max),ol=ol||pl.left>=0,ul=!0,An()}}function nn(){let e,t;tn=!0,1==M&&(Sl>0?(El=Dl[0]=0,zl=Dl[1]=Sl-1,e=g[0][El],t=g[0][zl],2==je?(e=El,t=zl):e==t&&(3==je?[e,t]=N(e,e,Ne.log,!1):4==je?[e,t]=j(e,e,Ne.log,!1):Ne.time?t=e+ne(86400/fe):[e,t]=q(e,t,U,!0))):(El=Dl[0]=e=null,zl=Dl[1]=t=null)),On(De,e,t)}function on(e,t,l,n,i,o){e??=a,l??=Pe,n??="butt",i??=a,o??="round",e!=Tl&&(J.strokeStyle=Tl=e),i!=Pl&&(J.fillStyle=Pl=i),t!=Wl&&(J.lineWidth=Wl=t),o!=ql&&(J.lineJoin=ql=o),n!=Kl&&(J.lineCap=Kl=n),l!=Jl&&J.setLineDash(Jl=l)}function sn(e,t,l,n){t!=Pl&&(J.fillStyle=Pl=t),e!=Xl&&(J.font=Xl=e),l!=Zl&&(J.textAlign=Zl=l),n!=$l&&(J.textBaseline=$l=n)}function rn(e,t,l,n,i=0){if(n.length>0&&e.auto(k,tn)&&(null==t||null==t.min)){let t=K(El,0),o=K(zl,n.length-1),s=null==l.min?3==e.distr?function(e,t,l){let n=he,i=-he;for(let o=t;l>=o;o++){let t=e[o];null!=t&&t>0&&(n>t&&(n=t),t>i&&(i=t))}return[n==he?1:n,i==-he?10:i]}(n,t,o):function(e,t,l,n){let i=he,o=-he;if(1==n)i=e[t],o=e[l];else if(-1==n)i=e[l],o=e[t];else for(let n=t;l>=n;n++){let t=e[n];null!=t&&(i>t&&(i=t),t>o&&(o=t))}return[i,o]}(n,t,o,i):[l.min,l.max];e.min=oe(e.min,l.min=s[0]),e.max=se(e.max,l.max=s[1])}}function un(e,t){let l=t?de[e].points:de[e];l._stroke=l.stroke(k,e),l._fill=l.fill(k,e)}function an(e,t){let l=t?de[e].points:de[e],{stroke:n,fill:i,clip:o,flags:s,_stroke:r=l._stroke,_fill:u=l._fill,_width:a=l.width}=l._paths;a=Se(a*y,3);let f=null,c=a%2/2;t&&null==u&&(u=a>0?"#fff":r);let h=1==l.pxAlign&&c>0;if(h&&J.translate(c,c),!t){let e=jt-a/2,t=Ut-a/2,l=Bt+a,n=Vt+a;f=new Path2D,f.rect(e,t,l,n)}t?cn(r,a,l.dash,l.cap,u,n,i,s,o):function(e,t,l,n,i,o,s,r,u,a,f){let c=!1;Me.forEach(((h,d)=>{if(h.series[0]==e){let e,p=de[h.series[1]],m=g[h.series[1]],x=(p._paths||Te).band;We(x)&&(x=1==h.dir?x[0]:x[1]);let w=null;p.show&&x&&function(e,t,l){for(t=K(t,0),l=K(l,e.length-1);l>=t;){if(null!=e[t])return!0;t++}return!1}(m,El,zl)?(w=h.fill(k,d)||o,e=p._paths.clip):x=null,cn(t,l,n,i,w,s,r,u,a,f,e,x),c=!0}})),c||cn(t,l,n,i,o,s,r,u,a,f)}(e,r,a,l.dash,l.cap,u,n,i,s,f,o),h&&J.translate(-c,-c)}k.setData=ln;const fn=3;function cn(e,t,l,n,i,o,s,r,u,a,f,c){on(e,t,l,n,i),(u||a||c)&&(J.save(),u&&J.clip(u),a&&J.clip(a)),c?(r&fn)==fn?(J.clip(c),f&&J.clip(f),dn(i,s),hn(e,o,t)):2&r?(dn(i,s),J.clip(c),hn(e,o,t)):1&r&&(J.save(),J.clip(c),f&&J.clip(f),dn(i,s),J.restore(),hn(e,o,t)):(dn(i,s),hn(e,o,t)),(u||a||c)&&J.restore()}function hn(e,t,l){l>0&&(t instanceof Map?t.forEach(((e,t)=>{J.strokeStyle=Tl=t,J.stroke(e)})):null!=t&&e&&J.stroke(t))}function dn(e,t){t instanceof Map?t.forEach(((e,t)=>{J.fillStyle=Pl=t,J.fill(e)})):null!=t&&e&&J.fill(t)}function pn(e,t,l,n,i,o,s,r,u,a){let f=s%2/2;1==le&&J.translate(f,f),on(r,s,u,a,r),J.beginPath();let c,h,d,p,m=i+(0==n||3==n?-o:o);0==l?(h=i,p=m):(c=i,d=m);for(let n=0;e.length>n;n++)null!=t[n]&&(0==l?c=d=e[n]:h=p=e[n],J.moveTo(c,h),J.lineTo(d,p));J.stroke(),1==le&&J.translate(-f,-f)}function mn(e){let t=!0;return ge.forEach(((l,n)=>{if(!l.show)return;let i=ve[l.scale];if(null==i.min)return void(l._show&&(t=!1,l._show=!1,xn(!1)));l._show||(t=!1,l._show=!0,xn(!1));let o=l.side,s=o%2,{min:r,max:u}=i,[a,f]=function(e,t,l,n){let i,o=ge[e];if(n>0){let s=o._space=o.space(k,e,t,l,n);i=Ul(t,l,o._incrs=o.incrs(k,e,t,l,n,s),n,s)}else i=[0,0];return o._found=i}(n,r,u,0==s?Yt:Ct);if(0==f)return;let c=l._splits=l.splits(k,n,r,u,a,f,2==i.distr),h=2==i.distr?c.map((e=>en[e])):c,d=2==i.distr?en[c[1]]-en[c[0]]:a,p=l._values=l.values(k,l.filter(k,h,n,f,d),n,f,d);l._rotate=2==o?l.rotate(k,p,n,f):0;let m=l._size;l._size=ie(l.size(k,p,n,e)),null!=m&&l._size!=m&&(t=!1)})),t}function gn(e){let t=!0;return yl.forEach(((l,n)=>{let i=l(k,n,vl,e);i!=Ml[n]&&(t=!1),Ml[n]=i})),t}function xn(e){de.forEach(((t,l)=>{l>0&&(t._paths=null,e&&(1==M?(t.min=null,t.max=null):t.facets.forEach((e=>{e.min=null,e.max=null}))))}))}let wn,_n,bn,vn,kn,yn,Mn,Sn,En,zn,Dn,Tn,Pn=!1;function An(){Pn||(Oe(Wn),Pn=!0)}function Wn(){Jt&&(function(){let e=Ge(ve,He);for(let t in e){let l=e[t],n=Ve[t];if(null!=n&&null!=n.min)Ie(l,n),t==De&&xn(!0);else if(t!=De||2==M)if(0==Sl&&null==l.from){let e=l.range(k,null,null,t);l.min=e[0],l.max=e[1]}else l.min=he,l.max=-he}if(Sl>0){de.forEach(((t,l)=>{if(1==M){let n=t.scale,i=e[n],o=Ve[n];if(0==l){let e=i.range(k,i.min,i.max,n);i.min=e[0],i.max=e[1],El=L(i.min,g[0]),zl=L(i.max,g[0]),zl-El>1&&(i.min>g[0][El]&&El++,g[0][zl]>i.max&&zl--),t.min=en[El],t.max=en[zl]}else t.show&&t.auto&&rn(i,o,t,g[l],t.sorted);t.idxs[0]=El,t.idxs[1]=zl}else if(l>0&&t.show&&t.auto){let[n,i]=t.facets,o=n.scale,s=i.scale,[r,u]=g[l];rn(e[o],Ve[o],n,r,n.sorted),rn(e[s],Ve[s],i,u,i.sorted),t.min=i.min,t.max=i.max}}));for(let t in e){let l=e[t],n=Ve[t];if(null==l.from&&(null==n||null==n.min)){let e=l.range(k,l.min==he?null:l.min,l.max==-he?null:l.max,t);l.min=e[0],l.max=e[1]}}}for(let t in e){let l=e[t];if(null!=l.from){let n=e[l.from];if(null==n.min)l.min=l.max=null;else{let e=l.range(k,n.min,n.max,t);l.min=e[0],l.max=e[1]}}}let t={},l=!1;for(let n in e){let i=e[n],o=ve[n];if(o.min!=i.min||o.max!=i.max){o.min=i.min,o.max=i.max;let e=o.distr;o._min=3==e?ae(o.min):4==e?ce(o.min,o.asinh):o.min,o._max=3==e?ae(o.max):4==e?ce(o.max,o.asinh):o.max,t[n]=l=!0}}if(l){de.forEach(((e,l)=>{2==M?l>0&&t.y&&(e._paths=null):t[e.scale]&&(e._paths=null)}));for(let e in t)Qt=!0,xi("setScale",e);pl.show&&pl.left>=0&&(ol=ul=!0)}for(let e in Ve)Ve[e]=null}(),Jt=!1),Qt&&(function(){let e=!1,t=0;for(;!e;){t++;let l=mn(t),n=gn(t);e=t==hl||l&&n,e||(cl(k.width,k.height),$t=!0)}}(),Qt=!1),$t&&(z(Z,s,Ft),z(Z,i,Ht),z(Z,l,Yt),z(Z,n,Ct),z($,s,Ft),z($,i,Ht),z($,l,Yt),z($,n,Ct),z(X,l,At),z(X,n,Wt),V.width=ne(At*y),V.height=ne(Wt*y),ge.forEach((({_el:t,_show:l,_size:n,_pos:i,side:o})=>{if(null!=t)if(l){let l=o%2==1;z(t,l?"left":"top",i-(3===o||0===o?n:0)),z(t,l?"width":"height",n),z(t,l?"top":"left",l?Ht:Ft),z(t,l?"height":"width",l?Ct:Yt),E(t,e)}else S(t,e)})),Tl=Pl=Wl=ql=Kl=Xl=Zl=$l=Jl=null,Ql=1,li(!0),xi("setSize"),$t=!1),At>0&&Wt>0&&(J.clearRect(0,0,V.width,V.height),xi("drawClear"),Re.forEach((e=>e())),xi("draw")),Gn.show&&rl&&(Ln(Gn),rl=!1),pl.show&&ol&&(ei(null,!0,!1),ol=!1),et.show&&et.live&&ul&&($n(),ul=!1),R||(R=!0,k.status=1,xi("ready")),tn=!1,Pn=!1}function Yn(e,t){let l=ve[e];if(null==l.from){if(0==Sl){let n=l.range(k,t.min,t.max,e);t.min=n[0],t.max=n[1]}if(t.min>t.max){let e=t.min;t.min=t.max,t.max=e}if(Sl>1&&null!=t.min&&null!=t.max&&1e-16>t.max-t.min)return;e==De&&2==l.distr&&Sl>0&&(t.min=L(t.min,g[0]),t.max=L(t.max,g[0]),t.min==t.max&&t.max++),Ve[e]=t,Jt=!0,An()}}k.redraw=(e,t)=>{Qt=t||!1,!1!==e?On(De,Ne.min,Ne.max):An()},k.setScale=Yn;let Cn=!1;const Fn=pl.drag;let Hn=Fn.x,Rn=Fn.y;pl.show&&(pl.x&&(wn=T("u-cursor-x",$)),pl.y&&(_n=T("u-cursor-y",$)),0==Ne.ori?(bn=wn,vn=_n):(bn=_n,vn=wn),Dn=pl.left,Tn=pl.top);const Gn=k.select=Ie({show:!0,over:!0,left:0,width:0,top:0,height:0},u.select),In=Gn.show?T("u-select",Gn.over?$:Z):null;function Ln(e,t){if(Gn.show){for(let t in e)Gn[t]=e[t],t in oi&&z(In,t,e[t]);!1!==t&&xi("setSelect")}}function On(e,t,l){Yn(e,{min:t,max:l})}function Nn(t,l,n,i){null!=l.focus&&function(e){if(e!=Bn){let t=null==e,l=1!=xl.alpha;de.forEach(((n,i)=>{let o=t||0==i||i==e;n._focus=t?null:o,l&&function(e,t){de[e].alpha=t,pl.show&&_l[e]&&(_l[e].style.opacity=t),tt&&at[e]&&(at[e].style.opacity=t)}(i,o?1:xl.alpha)})),Bn=e,l&&An()}}(t),null!=l.show&&de.forEach(((n,i)=>{0>=i||t!=i&&null!=t||(n.show=l.show,function(t){let l=tt?at[t]:null;de[t].show?l&&E(l,e):(l&&S(l,e),_l.length>1&&A(_l[t],-10,-10,Yt,Ct))}(i),On(2==M?n.facets[1].scale:n.scale,null,null),An())})),!1!==n&&xi("setSeries",t,l),i&&ki("setSeries",k,t,l)}let jn,Un,Bn;k.setSelect=Ln,k.setSeries=Nn,k.addBand=function(e,t){e.fill=me(e.fill||null),e.dir=K(e.dir,-1),Me.splice(t=null==t?Me.length:t,0,e)},k.setBand=function(e,t){Ie(Me[e],t)},k.delBand=function(e){null==e?Me.length=0:Me.splice(e,1)};const Vn={focus:!0};function Jn(e,t,l){let n=ve[t];l&&(e=e/y-(1==n.ori?Ht:Ft));let i=Yt;1==n.ori&&(i=Ct,e=i-e),-1==n.dir&&(e=i-e);let o=n._min,s=o+e/i*(n._max-o),r=n.distr;return 3==r?re(10,s):4==r?((e,t=1)=>Q.sinh(e)*t)(s,n.asinh):s}function qn(e,t){z(In,s,Gn.left=e),z(In,l,Gn.width=t)}function Kn(e,t){z(In,i,Gn.top=e),z(In,n,Gn.height=t)}tt&&wl&&Dt(p,ot,(e=>{pl._lock||(ml(e),null!=Bn&&Nn(null,Vn,!0,_i.setSeries))})),k.valToIdx=e=>L(e,g[0]),k.posToIdx=function(e,t){return L(Jn(e,De,t),g[0],El,zl)},k.posToVal=Jn,k.valToPos=(e,t,l)=>0==ve[t].ori?W(e,ve[t],l?Bt:Yt,l?jt:0):C(e,ve[t],l?Vt:Ct,l?Ut:0),k.batch=function(e){e(k),An()},k.setCursor=(e,t,l)=>{Dn=e.left,Tn=e.top,ei(null,t,l)};let Xn=0==Ne.ori?qn:Kn,Zn=1==Ne.ori?qn:Kn;function $n(e,t){null!=e&&(e.idxs?e.idxs.forEach(((e,t)=>{Qe[t]=e})):(e=>void 0===e)(e.idx)||Qe.fill(e.idx),et.idx=Qe[0]);for(let e=0;de.length>e;e++)(e>0||1==M&&!ct)&&Qn(e,Qe[e]);tt&&et.live&&function(){if(tt&&et.live)for(let e=2==M?1:0;de.length>e;e++){if(0==e&&ct)continue;let t=et.values[e],l=0;for(let n in t)ft[e][l++].firstChild.nodeValue=t[n]}}(),ul=!1,!1!==t&&xi("setLegend")}function Qn(e,t){let l,n=de[e],i=0==e&&2==je?en:g[e];ct?l=n.values(k,e,t)??ht:(l=n.value(k,null==t?null:i[t],e,t),l=null==l?ht:{_:l}),et.values[e]=l}function ei(e,t,l){let n;En=Dn,zn=Tn,[Dn,Tn]=pl.move(k,Dn,Tn),pl.show&&(bn&&A(bn,ne(Dn),0,Yt,Ct),vn&&A(vn,0,ne(Tn),Yt,Ct)),jn=he;let i=0==Ne.ori?Yt:Ct,o=1==Ne.ori?Yt:Ct;if(0>Dn||0==Sl||El>zl){n=null;for(let e=0;de.length>e;e++)e>0&&_l.length>1&&A(_l[e],-10,-10,Yt,Ct);wl&&Nn(null,Vn,!0,null==e&&_i.setSeries),et.live&&(Qe.fill(n),ul=!0)}else{let e,t,l;1==M&&(e=0==Ne.ori?Dn:Tn,t=Jn(e,De),n=L(t,g[0],El,zl),l=Ue(g[0][n],Ne,i,0));for(let e=2==M?1:0;de.length>e;e++){let s=de[e],r=Qe[e],u=1==M?g[e][r]:g[e][1][r],a=pl.dataIdx(k,e,n,t),f=1==M?g[e][a]:g[e][1][a];ul=ul||f!=u||a!=r,Qe[e]=a;let c=ye(a==n?l:Ue(1==M?g[0][a]:g[e][0][a],Ne,i,0),1);if(e>0&&s.show){let t,l,n=null==f?-10:ye(Be(f,1==M?ve[s.scale]:ve[s.facets[1].scale],o,0),1);if(wl&&n>=0&&1==M){let t=te(n-Tn);if(jn>t){let l=xl.bias;if(0!=l){let n=Jn(1==Ne.ori?Dn:Tn,s.scale),i=0>n?-1:1;i!=(0>f?-1:1)||(1==i?1==l?n>f:f>n:1==l?f>n:n>f)||(jn=t,Un=e)}else jn=t,Un=e}}if(0==Ne.ori?(t=c,l=n):(t=n,l=c),ul&&_l.length>1){Y(_l[e],pl.points.fill(k,e),pl.points.stroke(k,e));let n,i,o,s,r=!0,u=pl.points.bbox;if(null!=u){r=!1;let t=u(k,e);o=t.left,s=t.top,n=t.width,i=t.height}else o=t,s=l,n=i=pl.points.size(k,e);F(_l[e],n,i,r),A(_l[e],o,s,Yt,Ct)}}}}if(pl.idx=n,pl.left=Dn,pl.top=Tn,ul&&(et.idx=n,$n()),Gn.show&&Cn)if(null!=e){let[t,l]=_i.scales,[n,s]=_i.match,[r,u]=e.cursor.sync.scales,a=e.cursor.drag;if(Hn=a._x,Rn=a._y,Hn||Rn){let a,f,c,h,d,{left:p,top:m,width:g,height:x}=e.select,w=e.scales[t].ori,_=e.posToVal,b=null!=t&&n(t,r),v=null!=l&&s(l,u);b&&Hn?(0==w?(a=p,f=g):(a=m,f=x),c=ve[t],h=Ue(_(a,r),c,i,0),d=Ue(_(a+f,r),c,i,0),Xn(oe(h,d),te(d-h))):Xn(0,i),v&&Rn?(1==w?(a=p,f=g):(a=m,f=x),c=ve[l],h=Be(_(a,u),c,o,0),d=Be(_(a+f,u),c,o,0),Zn(oe(h,d),te(d-h))):Zn(0,o)}else si()}else{let e=te(En-kn),t=te(zn-yn);if(1==Ne.ori){let l=e;e=t,t=l}Hn=Fn.x&&e>=Fn.dist,Rn=Fn.y&&t>=Fn.dist;let l,n,s=Fn.uni;null!=s?Hn&&Rn&&(Hn=e>=s,Rn=t>=s,Hn||Rn||(t>e?Rn=!0:Hn=!0)):Fn.x&&Fn.y&&(Hn||Rn)&&(Hn=Rn=!0),Hn&&(0==Ne.ori?(l=Mn,n=Dn):(l=Sn,n=Tn),Xn(oe(l,n),te(n-l)),Rn||Zn(0,o)),Rn&&(1==Ne.ori?(l=Mn,n=Dn):(l=Sn,n=Tn),Zn(oe(l,n),te(n-l)),Hn||Xn(0,i)),Hn||Rn||(Xn(0,0),Zn(0,0))}if(Fn._x=Hn,Fn._y=Rn,null==e){if(l){if(null!=bi){let[e,t]=_i.scales;_i.values[0]=null!=e?Jn(0==Ne.ori?Dn:Tn,e):null,_i.values[1]=null!=t?Jn(1==Ne.ori?Dn:Tn,t):null}ki(f,k,Dn,Tn,Yt,Ct,n)}if(wl){let e=l&&_i.setSeries,t=xl.prox;null==Bn?jn>t||Nn(Un,Vn,!0,e):jn>t?Nn(null,Vn,!0,e):Un!=Bn&&Nn(Un,Vn,!0,e)}}!1!==t&&xi("setCursor")}k.setLegend=$n;let ti=null;function li(e=!1){e?ti=null:(ti=$.getBoundingClientRect(),xi("syncRect",ti))}function ni(e,t,l,n,i,o){pl._lock||Cn&&null!=e&&0==e.movementX&&0==e.movementY||(ii(e,t,l,n,i,o,0,!1,null!=e),null!=e?ei(null,!0,!0):ei(t,!0,!1))}function ii(e,t,l,n,i,o,s,r,u){if(null==ti&&li(!1),ml(e),null!=e)l=e.clientX-ti.left,n=e.clientY-ti.top;else{if(0>l||0>n)return Dn=-10,void(Tn=-10);let[e,s]=_i.scales,r=t.cursor.sync,[u,a]=r.values,[f,c]=r.scales,[h,d]=_i.match,p=t.axes[0].side%2==1,m=0==Ne.ori?Yt:Ct,g=1==Ne.ori?Yt:Ct,x=p?o:i,w=p?i:o,_=p?n:l,b=p?l:n;if(l=null!=f?h(e,f)?H(u,ve[e],m,0):-10:m*(_/x),n=null!=c?d(s,c)?H(a,ve[s],g,0):-10:g*(b/w),1==Ne.ori){let e=l;l=n,n=e}}u&&(l>1&&Yt-1>l||(l=ke(l,Yt)),n>1&&Ct-1>n||(n=ke(n,Ct))),r?(kn=l,yn=n,[Mn,Sn]=pl.move(k,l,n)):(Dn=l,Tn=n)}Object.defineProperty(k,"rect",{get:()=>(null==ti&&li(!1),ti)});const oi={width:0,height:0,left:0,top:0};function si(){Ln(oi,!1)}let ri,ui,ai,fi;function ci(e,t,l,n,i,o){Cn=!0,Hn=Rn=Fn._x=Fn._y=!1,ii(e,t,l,n,i,o,0,!0,!1),null!=e&&(Dt(h,b,hi,!1),ki(c,k,Mn,Sn,Yt,Ct,null));let{left:s,top:r,width:u,height:a}=Gn;ri=s,ui=r,ai=u,fi=a,si()}function hi(e,t,l,n,i,o){Cn=Fn._x=Fn._y=!1,ii(e,t,l,n,i,o,0,!1,!0);let{left:s,top:r,width:u,height:a}=Gn,f=u>0||a>0,c=ri!=s||ui!=r||ai!=u||fi!=a;if(f&&c&&Ln(Gn),Fn.setScale&&f&&c){let e=s,t=u,l=r,n=a;if(1==Ne.ori&&(e=r,t=a,l=s,n=u),Hn&&On(De,Jn(e,De),Jn(e+t,De)),Rn)for(let e in ve){let t=ve[e];e!=De&&null==t.from&&t.min!=he&&On(e,Jn(l+n,e),Jn(l,e))}si()}else pl.lock&&(pl._lock=!pl._lock,pl._lock||ei(null,!0,!1));null!=e&&(Tt(h,b),ki(h,k,Dn,Tn,Yt,Ct,null))}function di(e){pl._lock||(ml(e),nn(),si(),null!=e&&ki(m,k,Dn,Tn,Yt,Ct,null))}function pi(){ge.forEach(Vl),fl(k.width,k.height,!0)}G(x,v,pi);const mi={};mi.mousedown=ci,mi.mousemove=ni,mi.mouseup=hi,mi.dblclick=di,mi.setSeries=(e,t,l,n)=>{-1!=(l=(0,_i.match[2])(k,t,l))&&Nn(l,n,!0,!1)},pl.show&&(Dt(c,$,ci),Dt(f,$,ni),Dt(d,$,(e=>{ml(e),li(!1)})),Dt(p,$,(function(e){if(pl._lock)return;ml(e);let t=Cn;if(Cn){let e,t,l=!0,n=!0,i=10;0==Ne.ori?(e=Hn,t=Rn):(e=Rn,t=Hn),e&&t&&(l=i>=Dn||Dn>=Yt-i,n=i>=Tn||Tn>=Ct-i),e&&l&&(Dn=Mn>Dn?0:Yt),t&&n&&(Tn=Sn>Tn?0:Ct),ei(null,!0,!0),Cn=!1}Dn=-10,Tn=-10,ei(null,!0,!0),t&&(Cn=t)})),Dt(m,$,di),Al.add(k),k.syncRect=li);const gi=k.hooks=u.hooks||{};function xi(e,t,l){e in gi&&gi[e].forEach((e=>{e.call(null,k,t,l)}))}(u.plugins||[]).forEach((e=>{for(let t in e.hooks)gi[t]=(gi[t]||[]).concat(e.hooks[t])}));const wi=(e,t,l)=>l,_i=Ie({key:null,setSeries:!1,filters:{pub:_e,sub:_e},scales:[De,de[1]?de[1].scale:null],match:[be,be,wi],values:[null,null]},pl.sync);2==_i.match.length&&_i.match.push(wi),pl.sync=_i;const bi=_i.key,vi=sl(bi);function ki(e,t,l,n,i,o,s){_i.filters.pub(e,t,l,n,i,o,s)&&vi.pub(e,t,l,n,i,o,s)}function yi(){xi("init",u,g),ln(g||u.data,!1),Ve[De]?Yn(De,Ve[De]):nn(),rl=Gn.show&&(Gn.width>0||Gn.height>0),ol=ul=!0,fl(u.width,u.height)}return vi.sub(k),k.pub=function(e,t,l,n,i,o,s){_i.filters.sub(e,t,l,n,i,o,s)&&mi[e](null,t,l,n,i,o,s)},k.destroy=function(){vi.unsub(k),Al.delete(k),zt.clear(),I(x,v,pi),O.remove(),ot?.remove(),xi("destroy")},de.forEach(bl),ge.forEach((function(e,t){if(e._show=e.show,e.show){let l=ve[e.scale];null==l&&(e.scale=e.side%2?de[1].scale:De,l=ve[e.scale]);let n=l.time;e.size=me(e.size),e.space=me(e.space),e.rotate=me(e.rotate),We(e.incrs)&&e.incrs.forEach((e=>{!Ee.has(e)&&Ee.set(e,ze(e))})),e.incrs=me(e.incrs||(2==l.distr?lt:n?1==fe?mt:wt:nt)),e.splits=me(e.splits||(n&&1==l.distr?Ke:3==l.distr?Ot:4==l.distr?Nt:Lt)),e.stroke=me(e.stroke),e.grid.stroke=me(e.grid.stroke),e.ticks.stroke=me(e.ticks.stroke),e.border.stroke=me(e.border.stroke);let i=e.values;e.values=We(i)&&!We(i[0])?me(i):n?We(i)?kt(Je,vt(i,qe)):Ce(i)?function(e,t){let l=Xe(t);return(t,n)=>n.map((t=>l(e(t))))}(Je,i):i||Ze:i||It,e.filter=me(e.filter||(3>l.distr||10!=l.log?3==l.distr&&2==l.log?Kt:xe:qt)),e.font=Bl(e.font),e.labelFont=Bl(e.labelFont),e._size=e.size(k,null,t,0),e._space=e._rotate=e._incrs=e._found=e._splits=e._values=null,e._size>0&&(vl[t]=!0,e._el=T("u-axis",X))}})),_?_ instanceof HTMLElement?(_.appendChild(O),yi()):_(k,yi):yi(),k}Jl.assign=Ie,Jl.fmtNum=$,Jl.rangeNum=q,Jl.rangeLog=N,Jl.rangeAsinh=j,Jl.orient=rl,Jl.pxRatio=y,Jl.join=function(e,t){if(function(e){let t=e[0][0],l=t.length;for(let n=1;e.length>n;n++){let i=e[n][0];if(i.length!=l)return!1;if(i!=t)for(let e=0;l>e;e++)if(i[e]!=t[e])return!1}return!0}(e)){let t=e[0].slice();for(let l=1;e.length>l;l++)t.push(...e[l].slice(1));return function(e,t=100){const l=e.length;if(1>=l)return!0;let n=0,i=l-1;for(;i>=n&&null==e[n];)n++;for(;i>=n&&null==e[i];)i--;if(n>=i)return!0;const o=se(1,le((i-n+1)/t));for(let t=e[n],l=n+o;i>=l;l+=o){const n=e[l];if(null!=n){if(t>=n)return!1;t=n}}return!0}(t[0])||(t=function(e){let t=e[0],l=t.length,n=Array(l);for(let e=0;n.length>e;e++)n[e]=e;n.sort(((e,l)=>t[e]-t[l]));let i=[];for(let t=0;e.length>t;t++){let o=e[t],s=Array(l);for(let e=0;l>e;e++)s[e]=o[n[e]];i.push(s)}return i}(t)),t}let l=new Set;for(let t=0;e.length>t;t++){let n=e[t][0],i=n.length;for(let e=0;i>e;e++)l.add(n[e])}let n=[Array.from(l).sort(((e,t)=>e-t))],i=n[0].length,o=new Map;for(let e=0;i>e;e++)o.set(n[0][e],e);for(let l=0;e.length>l;l++){let s=e[l],r=s[0];for(let e=1;s.length>e;e++){let u=s[e],a=Array(i).fill(void 0),f=t?t[l][e]:1,c=[];for(let e=0;u.length>e;e++){let t=u[e],l=o.get(r[e]);null===t?0!=f&&(a[l]=t,2==f&&c.push(l)):a[l]=t}Le(a,c,i),n.push(a)}}return n},Jl.fmtDate=Xe,Jl.tzDate=function(e,t){let l;return"UTC"==t||"Etc/UTC"==t?l=new Date(+e+6e4*e.getTimezoneOffset()):t==Ze?l=e:(l=new Date(e.toLocaleString("en-US",{timeZone:t})),l.setMilliseconds(e.getMilliseconds())),l},Jl.sync=sl;{Jl.addGap=function(e,t,l){let n=e[e.length-1];n&&n[0]==t?n[1]=l:e.push([t,l])},Jl.clipGaps=cl;let e=Jl.paths={points:Sl};e.linear=Tl,e.stepped=function(e){const t=K(e.align,1),l=K(e.ascDesc,!1),n=K(e.alignGaps,0),i=K(e.extend,!1);return(e,o,s,r)=>rl(e,o,((u,a,f,c,h,d,p,m,g,x,w)=>{let _=u.pxRound,{left:b,width:v}=e.bbox,k=e=>_(d(e,c,x,m)),M=e=>_(p(e,h,w,g)),S=0==c.ori?xl:wl;const E={stroke:new Path2D,fill:null,clip:null,band:null,gaps:null,flags:1},z=E.stroke,D=c.dir*(0==c.ori?1:-1);s=O(f,s,r,1),r=O(f,s,r,-1);let T=M(f[1==D?s:r]),P=k(a[1==D?s:r]),A=P,W=P;i&&-1==t&&(W=b,S(z,W,T)),S(z,P,T);for(let e=1==D?s:r;e>=s&&r>=e;e+=D){let l=f[e];if(null==l)continue;let n=k(a[e]),i=M(l);1==t?S(z,n,T):S(z,A,i),S(z,n,i),T=i,A=n}let Y=A;i&&1==t&&(Y=b+v,S(z,Y,T));let[C,F]=ul(e,o);if(null!=u.fill||0!=C){let t=E.fill=new Path2D(z),l=M(u.fillTo(e,o,u.min,u.max,C));S(t,Y,l),S(t,W,l)}if(!u.spanGaps){let i=[];i.push(...hl(a,f,s,r,D,k,n));let h=u.width*y/2,d=l||1==t?h:-h,p=l||-1==t?-h:h;i.forEach((e=>{e[0]+=d,e[1]+=p})),E.gaps=i=u.gaps(e,o,s,r,i),E.clip=cl(i,c.ori,m,g,x,w)}return 0!=F&&(E.band=2==F?[fl(e,o,s,r,z,-1),fl(e,o,s,r,z,1)]:fl(e,o,s,r,z,F)),E}))},e.bars=function(e){const t=K((e=e||Te).size,[.6,he,1]),l=e.align||0,n=(e.gap||0)*y;let i=e.radius;i=null==i?[0,0]:"number"==typeof i?[i,0]:i;const o=me(i),s=1-t[0],r=K(t[1],he)*y,u=K(t[2],1)*y,a=K(e.disp,Te),f=K(e.each,(()=>{})),{fill:c,stroke:h}=a;return(e,t,i,d)=>rl(e,t,((p,m,g,x,w,_,b,v,k,M,S)=>{let E,z,D=p.pxRound;0==x.ori?[E,z]=o(e,t):[z,E]=o(e,t);const T=x.dir*(0==x.ori?1:-1),P=w.dir*(1==w.ori?1:-1);let A,W,Y=0==x.ori?_l:bl,C=0==x.ori?f:(e,t,l,n,i,o,s)=>{f(e,t,l,i,n,s,o)},[F,H]=ul(e,t),R=3==w.distr?1==F?w.max:w.min:0,G=b(R,w,S,k),I=D(p.width*y),L=!1,O=null,N=null,j=null,U=null;null==c||0!=I&&null==h||(L=!0,O=c.values(e,t,i,d),N=new Map,new Set(O).forEach((e=>{null!=e&&N.set(e,new Path2D)})),I>0&&(j=h.values(e,t,i,d),U=new Map,new Set(j).forEach((e=>{null!=e&&U.set(e,new Path2D)}))));let{x0:B,size:V}=a,J=!0;if(null!=B&&null!=V){m=B.values(e,t,i,d),2==B.unit&&(m=m.map((t=>e.posToVal(v+t*M,x.key,!0))));let l=V.values(e,t,i,d);W=2==V.unit?l[0]*M:_(l[0],x,M,v)-_(0,x,M,v),W/2>I||(I=0),W=D(pe(W-I,u,r)),A=1==T?-I/2:W+I/2}else{let e=M;if(m.length>1){let t=null;for(let l=0,n=1/0;m.length>l;l++)if(void 0!==g[l]){if(null!=t){let i=te(m[l]-m[t]);n>i&&(n=i,e=te(_(m[l],x,M,v)-_(m[t],x,M,v)))}t=l}}let t=e*s;W=e-t-n,W/2>I||(I=0),5>t+n&&(D=ge),W=D(pe(e-t,u,r)-I-n),A=(0==l?W/2:l==T?0:W)-l*T*n/2,W+I>e&&(J=!1)}const q={stroke:null,fill:null,clip:null,band:null,gaps:null,flags:3};let X;0!=H&&(q.band=new Path2D,X=D(b(1==H?w.max:w.min,w,S,k)));const Z=L?null:new Path2D,$=q.band;let{y0:Q,y1:ee}=a,ne=null;null!=Q&&null!=ee&&(g=ee.values(e,t,i,d),ne=Q.values(e,t,i,d));let ie=E*W,re=z*W;for(let l=1==T?i:d;l>=i&&d>=l;l+=T){let n=g[l];if(void 0===n)continue;let i=_(2!=x.distr||null!=a?m[l]:l,x,M,v),o=b(K(n,R),w,S,k);null!=ne&&null!=n&&(G=b(ne[l],w,S,k));let s=D(i-A),r=D(se(o,G)),u=D(oe(o,G)),f=r-u;if(null!=n){let i=0>n?re:ie,o=0>n?ie:re;L?(I>0&&null!=j[l]&&Y(U.get(j[l]),s,u+le(I/2),W,se(0,f-I),i,o),null!=O[l]&&Y(N.get(O[l]),s,u+le(I/2),W,se(0,f-I),i,o)):Y(Z,s,u+le(I/2),W,se(0,f-I),i,o),C(e,t,l,s-I/2,u,W+I,f)}0==H||null==n&&!J||(P*H==1?(r=u,u=X):(u=r,r=X),f=r-u,Y($,s-I/2,u,W+I,se(0,f),0,0))}return I>0?q.stroke=L?U:Z:L||(q._fill=0==p.width?p._fill:p._stroke??p._fill,q.width=0),q.fill=L?N:Z,q}))},e.spline=function(e){return function(e,t){const l=K(t?.alignGaps,0);return(t,n,i,o)=>rl(t,n,((s,r,u,a,f,c,h,d,p,m,g)=>{let x,w,_,b=s.pxRound,v=e=>b(c(e,a,m,d)),k=e=>b(h(e,f,g,p));0==a.ori?(x=ml,_=xl,w=yl):(x=gl,_=wl,w=Ml);const y=a.dir*(0==a.ori?1:-1);i=O(u,i,o,1),o=O(u,i,o,-1);let M=v(r[1==y?i:o]),S=M,E=[],z=[];for(let e=1==y?i:o;e>=i&&o>=e;e+=y)if(null!=u[e]){let t=v(r[e]);E.push(S=t),z.push(k(u[e]))}const D={stroke:e(E,z,x,_,w,b),fill:null,clip:null,band:null,gaps:null,flags:1},T=D.stroke;let[P,A]=ul(t,n);if(null!=s.fill||0!=P){let e=D.fill=new Path2D(T),l=k(s.fillTo(t,n,s.min,s.max,P));_(e,S,l),_(e,M,l)}if(!s.spanGaps){let e=[];e.push(...hl(r,u,i,o,y,v,l)),D.gaps=e=s.gaps(t,n,i,o,e),D.clip=cl(e,a.ori,d,p,m,g)}return 0!=A&&(D.band=2==A?[fl(t,n,i,o,T,-1),fl(t,n,i,o,T,1)]:fl(t,n,i,o,T,A)),D}))}(Pl,e)}}return Jl}();
// )!^!";