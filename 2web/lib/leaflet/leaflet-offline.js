!function(t,e){"object"==typeof exports&&"undefined"!=typeof module?e(exports,require("leaflet"),require("idb")):"function"==typeof define&&define.amd?define(["exports","leaflet","idb"],e):e((t="undefined"!=typeof globalThis?globalThis:t||self).LeafletOffline={},t.L,t.idb)}(this,(function(t,e,i){"use strict";function s(t,e,i,s){return new(i||(i=Promise))((function(o,n){function r(t){try{l(s.next(t))}catch(t){n(t)}}function a(t){try{l(s.throw(t))}catch(t){n(t)}}function l(t){var e;t.done?o(t.value):(e=t.value,e instanceof i?e:new i((function(t){t(e)}))).then(r,a)}l((s=s.apply(t,e||[])).next())}))}const o="tileStore",n="urlTemplate";let r;function a(){return r||(r=i.openDB("leaflet.offline",2,{upgrade(t,e){if(i.deleteDB("leaflet_offline"),i.deleteDB("leaflet_offline_areas"),e<1){const e=t.createObjectStore(o,{keyPath:"key"});e.createIndex(n,"urlTemplate"),e.createIndex("z","z")}}}),r)}function l(){return s(this,void 0,void 0,(function*(){return(yield a()).count(o)}))}function u(t){return s(this,void 0,void 0,(function*(){const e=yield fetch(t);if(!e.ok)throw new Error(`Request failed with status ${e.statusText}`);return e.blob()}))}function h(t,e){return s(this,void 0,void 0,(function*(){return(yield a()).put(o,Object.assign({blob:e},t))}))}function c(t,i){return e.Util.template(t,Object.assign(Object.assign({},i),{r:e.Browser.retina?"@2x":""}))}function d(t){return s(this,void 0,void 0,(function*(){return(yield a()).get(o,t).then((t=>t&&t.blob))}))}function f(t){return s(this,void 0,void 0,(function*(){const e=yield a();return void 0!==(yield e.getKey(o,t))}))}function g(){return s(this,void 0,void 0,(function*(){return(yield a()).clear(o)}))}class v extends e.TileLayer{createTile(t,i){const o=document.createElement("img");return e.DomEvent.on(o,"load",e.Util.bind(this._tileOnLoad,this,i,o)),e.DomEvent.on(o,"error",e.Util.bind(this._tileOnError,this,i,o)),(this.options.crossOrigin||""===this.options.crossOrigin)&&(o.crossOrigin=!0===this.options.crossOrigin?"":this.options.crossOrigin),o.alt="",o.setAttribute("role","presentation"),function(t,e){return s(this,void 0,void 0,(function*(){if(!(yield f(t)))return e;const i=yield d(t);return URL.createObjectURL(i)}))}(this._getStorageKey(t),this.getTileUrl(t)).then((t=>o.src=t)),o}_getStorageKey(t){return c(this._url,Object.assign(Object.assign(Object.assign({},t),this.options),{s:this.options.subdomains[0]}))}getTileUrls(t,i){var s;const o=[],n=function(t,i){const s=[];if(!t.min||!t.max)return s;const o=t.min.divideBy(i.x).floor(),n=t.max.divideBy(i.x).floor();for(let t=o.y;t<=n.y;t+=1)for(let i=o.x;i<=n.x;i+=1)s.push(new e.Point(i,t));return s}(t,this.getTileSize());for(let t=0;t<n.length;t+=1){const e=n[t],r=Object.assign(Object.assign({},this.options),{x:e.x,y:e.y,z:i+(this.options.zoomOffset||0)});o.push({key:c(this._url,Object.assign(Object.assign({},r),{s:null===(s=this.options.subdomains)||void 0===s?void 0:s[0]})),url:c(this._url,Object.assign(Object.assign({},r),{s:this._getSubdomain(e)})),z:i,x:e.x,y:e.y,urlTemplate:this._url,createdAt:Date.now()})}return o}}function y(t,e){return new v(t,e)}window.L&&(window.L.tileLayer.offline=y);class p extends e.Control{constructor(t,e){super(e),this.status={storagesize:0,lengthToBeSaved:0,lengthSaved:0,lengthLoaded:0,_tilesforSave:[]},this._baseLayer=t,this.setStorageSize(),this.options=Object.assign({position:"topleft",saveText:"+",rmText:"-",maxZoom:19,saveWhatYouSee:!1,bounds:null,confirm:null,confirmRemoval:null,parallel:50,zoomlevels:void 0,alwaysDownload:!0},e)}setStorageSize(){return this.status.storagesize?Promise.resolve(this.status.storagesize):l().then((t=>(this.status.storagesize=t,this._baseLayer.fire("storagesize",this.status),t))).catch((()=>0))}getStorageSize(t){this.setStorageSize().then((e=>{t&&t(e)}))}setLayer(t){this._baseLayer=t}onAdd(){const t=e.DomUtil.create("div","savetiles leaflet-bar"),{options:i}=this;return this._createButton(i.saveText,"savetiles",t,this._saveTiles),this._createButton(i.rmText,"rmtiles",t,this._rmTiles),t}_createButton(t,i,s,o){const n=e.DomUtil.create("a",i,s);return n.innerHTML=t,n.href="#",n.ariaRoleDescription="button",e.DomEvent.on(n,"mousedown dblclick",e.DomEvent.stopPropagation).on(n,"click",e.DomEvent.stop).on(n,"click",o,this).on(n,"click",this._refocusOnMap,this),n}_saveTiles(){const t=this._calculateTiles();this._resetStatus(t);const e=()=>s(this,void 0,void 0,(function*(){this._baseLayer.fire("savestart",this.status);const e=()=>s(this,void 0,void 0,(function*(){const i=t.shift();if(void 0===i)return Promise.resolve();const s=yield this._loadTile(i);return s&&(yield this._saveTile(i,s)),e()})),i=Math.min(t.length,this.options.parallel);for(let t=0;t<i;t+=1)e()}));this.options.confirm?this.options.confirm(this.status,e):e()}_calculateTiles(){let t=[],i=[];if(this.options.saveWhatYouSee){const t=this._map.getZoom();if(t<5)throw new Error("It's not possible to save with zoom below level 5.");const{maxZoom:e}=this.options;for(let s=t;s<=e;s+=1)i.push(s)}else i=this.options.zoomlevels||[this._map.getZoom()];const s=this.options.bounds||this._map.getBounds();for(let o=0;o<i.length;o+=1){const n=e.bounds(this._map.project(s.getNorthWest(),i[o]),this._map.project(s.getSouthEast(),i[o]));t=t.concat(this._baseLayer.getTileUrls(n,i[o]))}return t}_resetStatus(t){this.status={lengthLoaded:0,lengthToBeSaved:t.length,lengthSaved:0,_tilesforSave:t,storagesize:this.status.storagesize}}_loadTile(t){return s(this,void 0,void 0,(function*(){let e;return!0!==this.options.alwaysDownload&&!1!==(yield f(t.key))||(e=yield u(t.url),this.status.lengthLoaded+=1),this.status.lengthLoaded+=1,this._baseLayer.fire("loadtileend",this.status),this.status.lengthLoaded===this.status.lengthToBeSaved&&this._baseLayer.fire("loadend",this.status),e}))}_saveTile(t,e){return s(this,void 0,void 0,(function*(){yield h(t,e),this.status.lengthSaved+=1,this._baseLayer.fire("savetileend",this.status),this.status.lengthSaved===this.status.lengthToBeSaved&&(this._baseLayer.fire("saveend",this.status),this.setStorageSize())}))}_rmTiles(){const t=()=>{g().then((()=>{this.status.storagesize=0,this._baseLayer.fire("tilesremoved"),this._baseLayer.fire("storagesize",this.status)}))};this.options.confirmRemoval?this.options.confirmRemoval(this.status,t):t()}}function m(t,e){return new p(t,e)}window.L&&(window.L.control.savetiles=m),t.downloadTile=u,t.getBlobByKey=d,t.getStorageInfo=function(t){return s(this,void 0,void 0,(function*(){const e=IDBKeyRange.only(t);return(yield a()).getAllFromIndex(o,n,e)}))},t.getStorageLength=l,t.getStoredTilesAsJson=function(t,i){const s={type:"FeatureCollection",features:[]};for(let o=0;o<i.length;o+=1){const n=new e.Point(i[o].x*t.x,i[o].y*t.y),r=new e.Point(n.x+t.x,n.y+t.y),a=e.CRS.EPSG3857.pointToLatLng(n,i[o].z),l=e.CRS.EPSG3857.pointToLatLng(r,i[o].z);s.features.push({type:"Feature",properties:i[o],geometry:{type:"Polygon",coordinates:[[[a.lng,a.lat],[l.lng,a.lat],[l.lng,l.lat],[a.lng,l.lat],[a.lng,a.lat]]]}})}return s},t.removeTile=function(t){return s(this,void 0,void 0,(function*(){return(yield a()).delete(o,t)}))},t.saveTile=h,t.savetiles=m,t.tileLayerOffline=y,t.truncate=g}));