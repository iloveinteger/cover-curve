importScripts("cover_curve.js");

const FUNCTIONS = {sin:Math.sin,cos:Math.cos,tan:Math.tan,asin:Math.asin,acos:Math.acos,atan:Math.atan,exp:Math.exp,log:Math.log,sqrt:Math.sqrt,abs:Math.abs,floor:Math.floor,ceil:Math.ceil};
const CONSTANTS = {pi:Math.PI,e:Math.E};

function makeFunction(expression) {
  const source=expression.replace(/\s+/g,"");
  if(!source) throw new Error("Enter a function expression.");
  let pos=0,currentX=0;
  const peek=()=>source[pos]||"";
  function primary(){
    if(peek()==="("){pos++;const v=additive();if(peek()!==")")throw new Error("Expected ')'.");pos++;return v;}
    const num=source.slice(pos).match(/^(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?/);
    if(num){pos+=num[0].length;return Number(num[0]);}
    const id=source.slice(pos).match(/^[A-Za-z_][A-Za-z0-9_]*/);
    if(!id)throw new Error("Unexpected token at position "+pos+".");
    pos+=id[0].length;
    const name=id[0];
    if(name==="x")return currentX;
    if(Object.prototype.hasOwnProperty.call(CONSTANTS,name))return CONSTANTS[name];
    if(Object.prototype.hasOwnProperty.call(FUNCTIONS,name)){
      if(peek()!=="(")throw new Error("Expected '(' after "+name+".");
      pos++;const v=additive();if(peek()!==")")throw new Error("Expected ')'.");pos++;
      const out=FUNCTIONS[name](v);if(!Number.isFinite(out))throw new Error("Function '"+name+"' returned a non-finite value.");return out;
    }
    throw new Error("Unknown identifier '"+name+"'.");
  }
  function unary(){if(peek()==="+"){pos++;return unary();}if(peek()==="-"){pos++;return -unary();}return primary();}
  function power(){const v=unary();if(peek()==="^"){pos++;return Math.pow(v,power());}return v;}
  function multiplicative(){let v=power();while(peek()==="*"||peek()==="/"){const op=peek();pos++;const r=power();v=op==="*"?v*r:v/r;}return v;}
  function additive(){let v=multiplicative();while(peek()==="+"||peek()==="-"){const op=peek();pos++;const r=multiplicative();v=op==="+"?v+r:v-r;}return v;}
  return x=>{currentX=x;pos=0;const v=additive();if(pos!==source.length||!Number.isFinite(v))throw new Error("Invalid or non-finite function value.");return v;};
}

let wasmModule=null;
CoverCurve().then(module=>{wasmModule=module;postMessage({type:"ready"});}).catch(error=>postMessage({type:"load-error",error:String(error)}));

onmessage=event=>{
  if(!wasmModule){postMessage({type:"error",error:"Solver is still loading."});return;}
  try{
    const {expression,a,b,n}=event.data;
    const result=wasmModule.solve(makeFunction(expression),a,b,n);
    postMessage({type:"result",value:result.value,breakpoints:result.breakpoints,segments:result.segments,error:result.error});
  }catch(error){postMessage({type:"error",error:error.message||String(error)});}
};