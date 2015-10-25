namespace PPL2OpenPPL_CS
{
    partial class frmPrincipal
    {
        /// <summary>
        /// Variable del diseñador requerida.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén utilizando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben eliminar; false en caso contrario, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido del método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmPrincipal));
            this.btnSalir = new System.Windows.Forms.Button();
            this.btnEjecutar = new System.Windows.Forms.Button();
            this.lb = new System.Windows.Forms.ListBox();
            this.Label2 = new System.Windows.Forms.Label();
            this.btnSelFichero = new System.Windows.Forms.Button();
            this.txtFichero = new System.Windows.Forms.TextBox();
            this.Label1 = new System.Windows.Forms.Label();
            this.ofd = new System.Windows.Forms.OpenFileDialog();
            this.SuspendLayout();
            // 
            // btnSalir
            // 
            this.btnSalir.Location = new System.Drawing.Point(613, 488);
            this.btnSalir.Name = "btnSalir";
            this.btnSalir.Size = new System.Drawing.Size(75, 44);
            this.btnSalir.TabIndex = 13;
            this.btnSalir.Text = "Exit";
            this.btnSalir.UseVisualStyleBackColor = true;
            this.btnSalir.Click += new System.EventHandler(this.btnSalir_Click);
            // 
            // btnEjecutar
            // 
            this.btnEjecutar.Location = new System.Drawing.Point(12, 488);
            this.btnEjecutar.Name = "btnEjecutar";
            this.btnEjecutar.Size = new System.Drawing.Size(75, 44);
            this.btnEjecutar.TabIndex = 12;
            this.btnEjecutar.Text = "Run";
            this.btnEjecutar.UseVisualStyleBackColor = true;
            this.btnEjecutar.Click += new System.EventHandler(this.btnEjecutar_Click);
            // 
            // lb
            // 
            this.lb.FormattingEnabled = true;
            this.lb.Location = new System.Drawing.Point(12, 62);
            this.lb.Name = "lb";
            this.lb.Size = new System.Drawing.Size(676, 420);
            this.lb.TabIndex = 11;
            // 
            // Label2
            // 
            this.Label2.AutoSize = true;
            this.Label2.Location = new System.Drawing.Point(12, 46);
            this.Label2.Name = "Label2";
            this.Label2.Size = new System.Drawing.Size(59, 15);
            this.Label2.TabIndex = 10;
            this.Label2.Text = "Progress:";
            // 
            // btnSelFichero
            // 
            this.btnSelFichero.Location = new System.Drawing.Point(661, 10);
            this.btnSelFichero.Name = "btnSelFichero";
            this.btnSelFichero.Size = new System.Drawing.Size(28, 23);
            this.btnSelFichero.TabIndex = 9;
            this.btnSelFichero.Text = "...";
            this.btnSelFichero.UseVisualStyleBackColor = true;
            this.btnSelFichero.Click += new System.EventHandler(this.btnSelFichero_Click);
            // 
            // txtFichero
            // 
            this.txtFichero.Location = new System.Drawing.Point(103, 12);
            this.txtFichero.Name = "txtFichero";
            this.txtFichero.Size = new System.Drawing.Size(552, 20);
            this.txtFichero.TabIndex = 8;
            // 
            // Label1
            // 
            this.Label1.AutoSize = true;
            this.Label1.Location = new System.Drawing.Point(12, 15);
            this.Label1.Name = "Label1";
            this.Label1.Size = new System.Drawing.Size(85, 15);
            this.Label1.TabIndex = 7;
            this.Label1.Text = "File to convert:";
            // 
            // ofd
            // 
            this.ofd.FileName = "openFileDialog1";
            // 
            // frmPrincipal
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(695, 539);
            this.Controls.Add(this.btnSalir);
            this.Controls.Add(this.btnEjecutar);
            this.Controls.Add(this.lb);
            this.Controls.Add(this.Label2);
            this.Controls.Add(this.btnSelFichero);
            this.Controls.Add(this.txtFichero);
            this.Controls.Add(this.Label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "frmPrincipal";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "PPL 2 OpenPPL";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        internal System.Windows.Forms.Button btnSalir;
        internal System.Windows.Forms.Button btnEjecutar;
        internal System.Windows.Forms.ListBox lb;
        internal System.Windows.Forms.Label Label2;
        internal System.Windows.Forms.Button btnSelFichero;
        internal System.Windows.Forms.TextBox txtFichero;
        internal System.Windows.Forms.Label Label1;
        private System.Windows.Forms.OpenFileDialog ofd;
    }
}

