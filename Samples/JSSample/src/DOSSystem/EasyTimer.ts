export class EasyTimer {
    protected m_SavedTime = 0;
    protected m_Timeout = 0;

    public static get CurTime(): number {
        return Date.now();
    }

    public SaveTime(CurTime?: number) {
        if (CurTime === undefined)
            this.m_SavedTime = EasyTimer.CurTime;
        else
            this.m_SavedTime = CurTime;
    }
    public get SavedTime(): number {
        return this.m_SavedTime;
    }
    public IsTimeOut(TimeOut?: number): boolean {
        if (TimeOut == undefined)
            TimeOut = this.m_Timeout;
        let CurTime = EasyTimer.CurTime;
        if (CurTime - this.m_SavedTime >= TimeOut) {
            return true;
        }
        return false;
    }
    public set TimeOut(TimeOut: number) {
        this.m_SavedTime = EasyTimer.CurTime;
        this.m_Timeout = TimeOut;
    }
    public get TimeOut(): number {
        return this.m_Timeout;
    }

    public GetLeftTime(CurTime?: number): number {
        if (CurTime === undefined)
            CurTime = EasyTimer.CurTime;
        let Time = CurTime - this.m_SavedTime;
        if (Time >= this.m_Timeout)
            return 0;
        else
            return this.m_Timeout - Time;
    }

    public GetPastTime(CurTime?: number): number {
        if (CurTime === undefined)
            CurTime = EasyTimer.CurTime;
        return CurTime - this.m_SavedTime;
    }
}