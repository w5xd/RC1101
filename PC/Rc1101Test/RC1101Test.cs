﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Rc1101Test
{
    public partial class RC1101Test : Form
    {
        enum GenieObject_t { GENIE_OBJ_FORM = 10, GENIE_OBJ_LED_DIGITS=15, GENIE_OBJ_STRINGS = 17, GENIE_OBJ_GAUGE = 11 };

        private RadioPanelUsb.Finder finder;

        public RC1101Test()
        {
            InitializeComponent();
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void RC1101Test_Load(object sender, EventArgs e)
        {
            finder = new RadioPanelUsb.Finder();

            System.Collections.Generic.List<RadioPanelUsb.FrontPanel> panels =
                finder.listDevices();

            foreach (RadioPanelUsb.FrontPanel fp in panels)
            {
                FpListItem fpi = new FpListItem(fp);
                listBoxAvailable.Items.Add(fpi);
            }
        }

        private void listBoxAvailable_SelectedIndexChanged(object sender, EventArgs e)
        {
            bool enable = listBoxAvailable.SelectedIndex >= 0;
            buttonStart.Enabled = enable;
        }

        private FrontPanelExerciser fpe;

        private void buttonStart_Click(object sender, EventArgs e)
        {
            FpListItem fpi = listBoxAvailable.SelectedItem as FpListItem;
            if (null != fpi)
            {
                listBoxAvailable.Enabled = false;
                buttonStop.Enabled = true;
                buttonStart.Enabled = false;
                fpe = new FrontPanelExerciser();
                fpe.FrontPanel = fpi.FrontPanel;
                fpe.Run();
                buttonK3.Enabled = true;
            }
        }

        private void buttonStop_Click(object sender, EventArgs e)
        {
            if (fpe != null)
                fpe.Stop();
            m_keepGoing = false;
            buttonK3.Enabled = false;
            buttonStart.Enabled = true;
            buttonStop.Enabled = false;
            listBoxAvailable.Enabled = true;
        }

        private bool m_keepGoing = true;

        private void DoDisplay(FrontPanelExerciser fpe)
        {
            for (int g = 0; g < 100; g++)
            {
                int v = g;
                fpe.DoOnFp(new FrontPanelExerciser.CallFp(
                    (RadioPanelUsb.FrontPanel fp) =>
                    {
                        RadioPanelUsb.DisplayObject[] toDisplay = new RadioPanelUsb.DisplayObject[3];
                        for (int i = 0; i < toDisplay.Count(); i++)
                            toDisplay[i] = new RadioPanelUsb.DisplayObject();
                        toDisplay[0].objType = (ushort)GenieObject_t.GENIE_OBJ_STRINGS;
                        toDisplay[0].objIndex = 29;
                        toDisplay[0].value = 1;
                        toDisplay[1].objType = (ushort)GenieObject_t.GENIE_OBJ_GAUGE;
                        toDisplay[1].objIndex = 3;
                        toDisplay[1].value = (ushort)v;
                        toDisplay[2].objType = (ushort)GenieObject_t.GENIE_OBJ_GAUGE;
                        toDisplay[2].objIndex = 2;
                        toDisplay[2].value = (ushort)v;
                        fp.SetDisplayObjects(toDisplay);
                    }
                    ));
            }
            for (int g = 99; g >= 0; g--)
            {
                int v = g;
                fpe.DoOnFp(new FrontPanelExerciser.CallFp(
                    (RadioPanelUsb.FrontPanel fp) =>
                    {
                        RadioPanelUsb.DisplayObject[] toDisplay = new RadioPanelUsb.DisplayObject[3];
                        for (int i = 0; i < toDisplay.Count(); i++)
                            toDisplay[i] = new RadioPanelUsb.DisplayObject();
                        toDisplay[0].objType = (ushort)GenieObject_t.GENIE_OBJ_STRINGS;
                        toDisplay[0].objIndex = 29;
                        toDisplay[0].value = 1;
                        toDisplay[1].objType = (ushort)GenieObject_t.GENIE_OBJ_GAUGE;
                        toDisplay[1].objIndex = 3;
                        toDisplay[1].value = (ushort)v;
                        toDisplay[2].objType = (ushort)GenieObject_t.GENIE_OBJ_GAUGE;
                        toDisplay[2].objIndex = 2;
                        toDisplay[2].value = (ushort)v;
                        fp.SetDisplayObjects(toDisplay);
                    }
                    ));
            }
            if (m_keepGoing)
                fpe.DoOnFp((RadioPanelUsb.FrontPanel fp) =>
                {
                    DoDisplay(fpe);
                });
        }

        private void buttonK3_Click(object sender, EventArgs e)
        {
            buttonK3.Enabled = false;
            m_keepGoing = true;
            if (fpe != null)
            {
                fpe.DoOnFp(new FrontPanelExerciser.CallFp(
                    (RadioPanelUsb.FrontPanel fp) => {
                        RadioPanelUsb.DisplayObject[] toDisplay = new RadioPanelUsb.DisplayObject[1];
                        toDisplay[0] = new RadioPanelUsb.DisplayObject();
                        toDisplay[0].objIndex = 2;
                        toDisplay[0].value = 1;
                        toDisplay[0].objType = (ushort)GenieObject_t.GENIE_OBJ_FORM;
                        fp.SetDisplayObjects(toDisplay);

                        fp.SetEncoderMap(0, (ushort)GenieObject_t.GENIE_OBJ_LED_DIGITS, 8,
                                        (ushort)GenieObject_t.GENIE_OBJ_LED_DIGITS, 9, 1, 0, 9999999);
                        fp.SetEncoderMap(1, (ushort)GenieObject_t.GENIE_OBJ_LED_DIGITS, 10,
                                        (ushort)GenieObject_t.GENIE_OBJ_LED_DIGITS, 11, 1, 0, 999999);
                    }
                    ));

                DoDisplay(fpe);
             }
        }

        private void RC1101Test_FormClosing(object sender, FormClosingEventArgs e)
        {
            foreach (FpListItem fpi in listBoxAvailable.Items) {
                fpi.FrontPanel.Dispose();
            }
            if (fpe != null)
                fpe.Stop();
        }
    }

    class FpListItem
    {
        public FpListItem(RadioPanelUsb.FrontPanel fp)
        {
            m_fp = fp;
        }

        public String GetUsbSerialNumber() { return m_fp.GetUsbSerialNumber(); }

        public override String ToString()
        {
            string id = m_fp.GetIdString();
            if (String.IsNullOrEmpty(id))
                id = m_fp.GetUsbSerialNumber();
            return id;
        }

        public RadioPanelUsb.FrontPanel FrontPanel { get { return m_fp; } }

        private RadioPanelUsb.FrontPanel m_fp;
    }
}