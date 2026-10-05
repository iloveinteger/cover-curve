importScripts("cover_curve.js");

importScripts("expression.js");
let wasmModule=null;
CoverCurve().then(module=>{wasmModule=module;postMessage({type:"ready"});}).catch(error=>postMessage({type:"load-error",error:String(error)}));

onmessage=event=>{
  if(!wasmModule){postMessage({type:"error",error:"Solver is still loading."});return;}
  try{
    const {expression,a,b,n}=event.data;
    const result=wasmModule.solve(expression,a,b,n);
    postMessage({type:"result",value:result.value,breakpoints:result.breakpoints,segments:result.segments,error:result.error});
  }catch(error){postMessage({type:"error",error:error.message||String(error)});}
};