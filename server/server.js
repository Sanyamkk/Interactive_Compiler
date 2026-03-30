const express = require('express');
const cors = require('cors');
const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');

const app = express();
app.use(cors());
app.use(express.json());

const PORT = 3001;

app.post('/compile', (req, res) => {
    const code = req.body.code || "";
    const tempFile = path.join(__dirname, 'temp.txt');
    
    // Write code to a temporary file
    fs.writeFileSync(tempFile, code);
    
    // Execute the compiler
    const compilerPath = path.join(__dirname, '..', 'backend', 'compiler.exe');
    
    exec(`"${compilerPath}" "${tempFile}"`, (error, stdout, stderr) => {
        // We delete the temp file
        try { fs.unlinkSync(tempFile); } catch (e) {}
        
        // The compiler should always output JSON on stdout, even if there are syntax errors
        try {
            const result = JSON.parse(stdout);
            res.json(result);
        } catch (parseError) {
            console.error("Failed to parse compiler output:", stdout);
            res.status(500).json({
                error: "Compiler did not return valid JSON.",
                details: stdout || stderr || (error && error.message)
            });
        }
    });
});

app.listen(PORT, () => {
    console.log(`Compiler server running on http://localhost:${PORT}`);
});
