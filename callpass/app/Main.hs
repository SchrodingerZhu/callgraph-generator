module Main where
import Command
import Options.Applicative


  
main :: IO ()
main = do
  file <- execParser $ info commandParser mempty
  runCommand file
