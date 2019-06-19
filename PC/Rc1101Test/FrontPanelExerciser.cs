using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace Rc1101Test
{
    class FrontPanelExerciser
    {
        public delegate void CallFp(RadioPanelUsb.FrontPanel fp);
        private bool m_stop = false;
        private bool m_running = false;
        private RadioPanelUsb.FrontPanel m_frontPanel;
        public RadioPanelUsb.FrontPanel FrontPanel
            { set { m_frontPanel = value; } get { return m_frontPanel; } }

        public void Stop()
        {
            m_stop = true;
        }

        public void Run()
        {
            if (m_frontPanel == null)
                throw new Exception("Cannot run without FrontPanel");
            Thread t3 = new Thread(() => head());
            t3.Start();
        }

        public void DoOnFp(CallFp fp)
        {
            lock (m_queue)
            {
                m_queue.Add(fp);
            }
        }

        private const int NUMBER_OF_ENCODERS = 7;

        private uint[] m_encCenters = new uint[NUMBER_OF_ENCODERS];
        private List<CallFp> m_queue = new List<CallFp>();
        private void head()
        {
            m_running = true;
            m_frontPanel.ResetDisplayDefaults();
#if false
            m_frontPanel.SetEncoderMap(1, 15, 2, 15, 3, 10);
            m_frontPanel.SetEncoderCenter(1, 700000);
            RadioPanelUsb.DisplayObject []toDisplay = new RadioPanelUsb.DisplayObject[1];
            toDisplay[0] = new RadioPanelUsb.DisplayObject();
            toDisplay[0].objType = 10;
            toDisplay[0].objIndex = 2;
            toDisplay[0].value = 1;
            m_frontPanel.SetDisplayObjects(toDisplay);
#endif
            short[] enow = new short[NUMBER_OF_ENCODERS];
            short[] eprev = new short[NUMBER_OF_ENCODERS];
            ushort switches = 0; ushort switchesprev = 0;
            byte encswitches = 0; byte encswitchesprev = 0;
            var startTime = DateTime.UtcNow;
            uint okCount = 0;
            while (!m_stop)
            {
                CallFp doIt = null;
                lock (m_queue)
                {
                    if (m_queue.Any())
                    {
                        doIt = m_queue.First();
                        m_queue.RemoveAt(0);
                    }
                }
                if (doIt != null)
                    doIt(m_frontPanel);
                ushort numenc = 0;
                bool ok = m_frontPanel.GetInputState(enow, ref numenc, ref switches, ref encswitches);
                System.DateTime now = System.DateTime.Now;
                if (ok)
                {
                    int i = NUMBER_OF_ENCODERS - 1;
                    for (; i >= 0; i--)
                    {   // scan backwards
                        if (eprev[i] != enow[i])
                            break;
                    }
                    if (encswitches != encswitchesprev)
                    {
                        encswitchesprev += 0;
                    }
                    if ((i >= 0) ||
                        (switches != switchesprev))
                    {
                        uint[] update = new uint[i + 1];
                        for (int j = 0; j <= i; j++)
                            update[j] = (uint)(m_encCenters[j] + enow[j]);
                        ushort sw = switches;
                    }
                    switchesprev = switches;
                    encswitchesprev = encswitches;
                    okCount += 1;
                }
            }
#if DEBUG
            var stopTime = DateTime.UtcNow;
            var elapsed = stopTime - startTime;
            double msec = elapsed.TotalMilliseconds;
            double msecPerCall = msec / okCount;
#endif
            m_running = false;
        }
    }
}
