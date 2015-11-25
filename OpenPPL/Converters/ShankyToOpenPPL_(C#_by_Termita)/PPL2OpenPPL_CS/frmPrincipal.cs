using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;
using Microsoft.VisualBasic;

namespace PPL2OpenPPL_CS
{
    public partial class frmPrincipal : Form
    {
        public frmPrincipal()
        {
            InitializeComponent();
        }

        private void btnSalir_Click(System.Object sender, System.EventArgs e)
        {
            this.Close();
        }

        private void btnSelFichero_Click(System.Object sender, System.EventArgs e)
        {
            string ruta = txtFichero.Text.Trim();
            ofd.Filter = "All files (*.*)|*.*";
            if (ruta.Length == 0)
            {
                ofd.InitialDirectory = Application.StartupPath;
                ofd.FileName = "";
            }
            else
                ofd.FileName = ruta;
            if (ofd.ShowDialog() == DialogResult.OK)
                txtFichero.Text = ofd.FileName;
        }

        private void btnEjecutar_Click(System.Object sender, System.EventArgs e)
        {
            try
            {
                string fichero = txtFichero.Text.Trim();
                string ficheroSalida = fichero + ".oppl";
                if (File.Exists(ficheroSalida))
                    File.Delete(ficheroSalida);
                lb.Items.Clear();

                string[] lineas = File.ReadAllLines(fichero);
                cargarTablas();

                Dictionary<int, string> lineasNuevas = new Dictionary<int, string>();
                foreach (string linea in lineas)
                {
                    string l = linea.Trim().ToLower();
                    if (l.Length == 0)
                        continue;
                    if (lineasNuevas.Count == 0)
                    {
                        if (!(l.Split(' ')[0] == "preflop"))
                            continue;
                        anyadirLinea(lineasNuevas, "##f$preflop##\n");
                        anyadirLinea(lineasNuevas, "");
                    }
                    else
                    {
                        if ("flop,turn,river".Contains(l.Split(' ')[0]))
                        {
                            anyadirLinea(lineasNuevas, "\n##f$" + l.Split(' ')[0] + "##\n");
                            continue;
                        }
                        if (l.StartsWith("//"))
                            continue;
                        string lineaAntigua = "";
                        l = procesarEspeciales(corregirEspacios(l, new string[2] { "in s|ins", "in b|inb" }));
                        while (l != lineaAntigua)
                        {
                            lineaAntigua = l;
                            l = procesarLinea(l);
                        }
                        l = corregirManos(corregirEspacios(l, new string[3] { "hand = |hand$", "board = |board$", " Suited|Suited" }));
                        l = corregirEspacios(l, new string[5] { " . |.", "! =|!=", "< =|<=", "> =|>=", " %|%" });
                        l = corregirRaiseForce(l);
                        l = corregirBetSize(l);
                        anyadirLinea(lineasNuevas, l);
                    }
                }

                File.AppendAllLines(ficheroSalida, lineasNuevas.Values);

                MessageBox.Show("Process finished");
            }
            catch (Exception ex)
            {
                MessageBox.Show("ERROR: " + ex.Message);
            }
        }

        const string caracteresEspeciales = "<>=%()!.";
        private string procesarEspeciales(string linea)
        {
            string res = "";
            for (int i = 0; i <= linea.Length - 1; i++)
            {
                if (caracteresEspeciales.Contains("" + linea[i]))
                    res += " ";
                res += linea[i];
                if (caracteresEspeciales.Contains("" + linea[i]))
                    res += " ";
            }
            return res;
        }

        private string procesarLinea(string linea)
        {
            string[] palabras = linea.Trim().Split(' ');
            linea = "";
            foreach (string palabra in palabras)
            {
                string p = palabra.Replace("2nd", "second").Replace("3rd", "Third");
                if (tablaSimbolos.ContainsKey(p))
                    linea += " " + tablaSimbolos[p];
                else if (tablaVariables.ContainsKey(p.ToLower()))
                    linea += " " + tablaVariables[p.ToLower()];
                else
                    linea += " " + p;
            }
            return linea.Substring(1);
        }

        private string corregirBetSize(string l)
        {
            foreach (string c in new string[3] { "BetSize", "StackSize", "AmountToCall" })
                l = l.Replace(c + " =", c + " ~~");
            return l;
        }

        private string corregirRaiseForce(string l)
        {
            string[] p = l.Trim().Split(' ');
            if (p[p.Length - 1] == "FORCE")
            {
                if (p[p.Length - 3] == "Raise" & (Information.IsNumeric(p[p.Length - 2]) | p[p.Length - 2].EndsWith("%")))
                    l = l.Substring(0, l.LastIndexOf("Raise")) + "RaiseBy" + l.Substring(l.LastIndexOf("Raise") + 5);
                else if (p[p.Length - 3] == "Bet" & (Information.IsNumeric(p[p.Length - 2]) | p[p.Length - 2].EndsWith("%")))
                    l = l.Substring(0, l.LastIndexOf("Bet")) + "RaiseBy" + l.Substring(l.LastIndexOf("Bet") + 3);
                else if (p[p.Length - 2] == "Bet" & p[p.Length - 1] == "FORCE")
                    l = l.Substring(0, l.LastIndexOf("Bet")) + "Raise" + l.Substring(l.LastIndexOf("Bet") + 3);
            }
            return l;
        }

        private string corregirEspacios(string l, string[] correcciones)
        {
            l = l.Trim();
            for (int i = 10; i >= 2; i += -1)
                l = l.Replace(" ".PadLeft(i), " ");
            if (l.StartsWith("WHEN"))
            {
                if (l.EndsWith("FORCE"))
                    l = "\t" + l;
                else
                    l = "\n" + l;
                l = "\n" + l;
            }
            foreach (string c in correcciones)
            {
                string[] d = c.Split('|');
                l = l.Replace(d[0], d[1]);
            }
            return l;
        }

        private string corregirManos(string l)
        {
            string[] palabras = l.Split(' ');
            string res = "";
            bool bVer = false;
            foreach (string p in palabras)
            {
                if (p.Trim().Length == 0)
                    continue;
                if (p.StartsWith("hand$") | p.StartsWith("board$"))
                {
                    res += " " + p;
                    bVer = true;
                }
                else if (bVer)
                {
                    if ((p.Length < 3 | p.EndsWith("Suited")) & !(p == "OR" | p == ")"))
                        res += p;
                    else
                    {
                        res += " " + p;
                        bVer = false;
                    }
                }
                else
                    res += " " + p;
            }
            return res;
        }

        private void anyadirLinea(Dictionary<int, string> lineasNuevas, string linea)
        {
            if (!(linea.Trim().StartsWith("when") | linea.Trim().StartsWith("##")))
                lineasNuevas[lineasNuevas.Count] += " " + linea;
            else
                lineasNuevas.Add(lineasNuevas.Count + 1, linea);
            lb.Items.Add(linea);
            lb.SelectedIndex = lb.Items.Count - 1;
            Application.DoEvents();
        }

        Dictionary<string, string> tablaSimbolos = new Dictionary<string, string>();
        Dictionary<string, string> tablaVariables = new Dictionary<string, string>();
        private void cargarTablas()
        {
            string fichero = Application.StartupPath + "\\keys.txt";
            if (!File.Exists(fichero))
                throw new Exception("File " + fichero + " does not exists");
            tablaSimbolos.Clear();
            foreach (string l in File.ReadAllLines(fichero))
            {
                string[] v = l.Split(',');
                tablaSimbolos.Add(v[0].ToLower(), v[1]);
            }
            fichero = Application.StartupPath + "\\OpenPPL_Library.ohf";
            if (!File.Exists(fichero))
                throw new Exception("File " + fichero + " does not exists");
            tablaVariables.Clear();
            foreach (string linea in File.ReadAllLines(fichero))
            {
                string l = linea;
                l = l.Trim();
                if (l.StartsWith("##") & l.EndsWith("##"))
                {
                    l = l.Replace("##", "").Replace("f$", "");
                    if (!tablaVariables.ContainsKey(l.ToLower()))
                        tablaVariables.Add(l.ToLower(), l);
                }
            }
        }
    }
}
