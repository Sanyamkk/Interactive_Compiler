import React from 'react';

function Header({ loading, handleCompile }) {
  return (
    <header className="header">
      <h1>Interactive_Compiler Project</h1>
      <button onClick={handleCompile} disabled={loading} className="compile-btn">
        {loading ? 'Compiling...' : 'Run Compiler'}
      </button>
    </header>
  );
}

export default Header;
