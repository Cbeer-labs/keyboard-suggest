function x(){}function k(t,n){for(const e in n)t[e]=n[e];return t}function M(t){return!!t&&(typeof t=="object"||typeof t=="function")&&typeof t.then=="function"}function w(t){return t()}function S(){return Object.create(null)}function j(t){t.forEach(w)}function A(t){return typeof t=="function"}function B(t,n){return t!=t?n==n:t!==n||t&&typeof t=="object"||typeof t=="function"}function D(t){return Object.keys(t).length===0}function E(t,...n){if(t==null){for(const o of n)o(void 0);return x}const e=t.subscribe(...n);return e.unsubscribe?()=>e.unsubscribe():e}function F(t,n,e){t.$$.on_destroy.push(E(n,e))}function P(t,n,e,o){if(t){const c=y(t,n,e,o);return t[0](c)}}function y(t,n,e,o){return t[1]&&o?k(e.ctx.slice(),t[1](o(n))):e.ctx}function U(t,n,e,o){if(t[2]&&o){const c=t[2](o(e));if(n.dirty===void 0)return c;if(typeof c=="object"){const a=[],f=Math.max(n.dirty.length,c.length);for(let u=0;u<f;u+=1)a[u]=n.dirty[u]|c[u];return a}return n.dirty|c}return n.dirty}function v(t,n,e,o,c,a){if(c){const f=y(n,e,o,a);t.p(f,c)}}function G(t){if(t.ctx.length>32){const n=[],e=t.ctx.length/32;for(let o=0;o<e;o++)n[o]=-1;return n}return-1}function H(t){const n={};for(const e in t)e[0]!=="$"&&(n[e]=t[e]);return n}function I(t,n){const e={};n=new Set(n);for(const o in t)!n.has(o)&&o[0]!=="$"&&(e[o]=t[o]);return e}let l;function _(t){l=t}function b(){if(!l)throw new Error("Function called outside component initialization");return l}function J(t){b().$$.on_mount.push(t)}function K(t){b().$$.after_update.push(t)}function L(t){return b().$$.context.get(t)}function N(t,n){const e=t.$$.callbacks[n.type];e&&e.slice().forEach(o=>o.call(this,n))}const i=[],g=[];let s=[];const d=[],m=Promise.resolve();let p=!1;function O(){p||(p=!0,m.then(z))}function Q(){return O(),m}function q(t){s.push(t)}function R(t){d.push(t)}const h=new Set;let r=0;function z(){if(r!==0)return;const t=l;do{try{for(;r<i.length;){const n=i[r];r++,_(n),C(n.$$)}}catch(n){throw i.length=0,r=0,n}for(_(null),i.length=0,r=0;g.length;)g.pop()();for(let n=0;n<s.length;n+=1){const e=s[n];h.has(e)||(h.add(e),e())}s.length=0}while(i.length);for(;d.length;)d.pop()();p=!1,h.clear(),_(t)}function C(t){if(t.fragment!==null){t.update(),j(t.before_update);const n=t.dirty;t.dirty=[-1],t.fragment&&t.fragment.p(t.ctx,n),t.after_update.forEach(q)}}function T(t){const n=[],e=[];s.forEach(o=>t.indexOf(o)===-1?n.push(o):e.push(o)),e.forEach(o=>o()),s=n}export{T as A,l as B,w as C,i as D,O as E,K as a,g as b,P as c,U as d,F as e,b as f,G as g,_ as h,M as i,z as j,I as k,L as l,k as m,x as n,J as o,H as p,N as q,j as r,B as s,Q as t,v as u,R as v,A as w,S as x,D as y,q as z};
