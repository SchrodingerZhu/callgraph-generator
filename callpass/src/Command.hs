module Command where
import System.Process
import Options.Applicative
import System.IO
data Config = Config { getFile :: FilePath, sharedLib :: FilePath, outputPath :: FilePath}

commandParser :: Parser Config
commandParser = Config
    <$> strOption (
        long "source"
        <> short 's'
        <> metavar "TARGET"
        <> help "source file"
    )
    <*> strOption (
        long "lib"
        <> short 'l'
        <> metavar "SHARED_LIB"
        <> help "path to the shared library"
    )
    <*> strOption (
        long "output"
        <> short 'o'
        <> metavar "OUTPUT_FILE"
        <> help "path to the output file"
    )


runCommand :: Config -> IO ()
runCommand config = do
    (_, Just hout, _, _) <- createProcess (proc "clang++" ["-emit-llvm", "-c", (getFile config), "-o", "/dev/stdout"]) {
        std_out = CreatePipe
    }
    file <- openFile (outputPath config) WriteMode
    none <- openFile "/dev/null" WriteMode
    _ <- createProcess (proc "opt" ["-load", (sharedLib config), "-dumpcalls", "-f"]) {
        std_in = UseHandle hout,
        std_err = UseHandle file,
        std_out = UseHandle none
    }
    hClose hout
    hClose file
    hClose none



