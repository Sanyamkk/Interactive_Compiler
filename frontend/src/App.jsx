import React, { useState } from 'react';
import './App.css';

function App() {
  const [code, setCode] = useState("int a;\na = 5;\nprint(a);");
  const [results, setResults] = useState(null);
  const [loading, setLoading] = useState(false);

  const handleCompile = async () => {
    setLoading(true);
    try {
      const response = await fetch('http://localhost:3001/compile', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ code }),
      });
      const data = await response.json();
      setResults(data);
    } catch (err) {
      console.error(err);
      setResults({ errors: [{ phase: "Network", message: "Failed to connect to compile server." }] });
    }
    setLoading(false);
  };

  return (
    <div className="container">
      <header className="header">
        <h1>Compiler PBL 🚀</h1>
        <button onClick={handleCompile} disabled={loading} className="compile-btn">
          {loading ? 'Compiling...' : 'Run Compiler'}
        </button>
      </header>
      
      <div className="main-content">
        <div className="editor-pane pane">
          <h2>Source Code</h2>
          <textarea
            value={code}
            onChange={(e) => setCode(e.target.value)}
            className="code-editor"
            spellCheck="false"
          />
        </div>

        <div className="results-pane pane">
          <h2>Compilation Results</h2>
          {results ? (
            <div className="json-tree">
               {results.errors && results.errors.length > 0 && (
                <div className="section errors">
                  <h3>Errors Detected</h3>
                  <ul>
                    {results.errors.map((e, i) => (
                      <li key={i}>[{e.phase}] Line {e.line}: {e.message}</li>
                    ))}
                  </ul>
                </div>
               )}
               {/* Show success when no errors */}
               {(!results.errors || results.errors.length === 0) && (
                 <div className="section success" style={{color: '#4ade80'}}>
                   <h3>✅ Compilation Successful</h3>
                 </div>
               )}
               
               {results.ir && results.ir.length > 0 && (
                <div className="section ir">
                  <h3>Intermediate Representation</h3>
                  <pre>{results.ir.join('\n')}</pre>
                </div>
               )}
               {results.symbols && results.symbols.length > 0 && (
                <div className="section symbols">
                  <h3>Symbol Table</h3>
                  <ul>
                    {results.symbols.map((s, i) => (
                      <li key={i}>{s.name} : {s.type}</li>
                    ))}
                  </ul>
                </div>
               )}
               {results.tokens && results.tokens.length > 0 && (
                 <div className="section tokens">
                   <h3>Tokens</h3>
                   <div className="token-list">
                     {results.tokens.map((t, i) => (
                       <span key={i} className="token">{t.type} <br/><small>{t.value}</small></span>
                     ))}
                   </div>
                 </div>
               )}
            </div>
          ) : (
            <div className="placeholder">Run the compiler to see results here.</div>
          )}
        </div>
      </div>
    </div>
  );
}

export default App;
