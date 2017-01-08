import React,{Component,Proptypes} from  'react'
import {steps,gifts} from '../StaticResource/navBarTitle'
import {Form , Input , Select , Button , Checkbox , Radio , Tooltip , Icon, Steps,Spin} from 'antd'
const FormItem = Form.Item;
const Step = Steps.Step;
const RadioButton = Radio.Button;
const RadioGroup = Radio.Group;

export default class Myform extends Component {
    constructor(props) {
        super(props);
        this.makeSureIsVIP = this.makeSureIsVIP.bind(this);
        this.handleChange = this.handleChange.bind(this);
        this.doleGift = this.doleGift.bind(this);
        this.postInfo = this.postInfo.bind(this);
        this.stepBystep = this.stepBystep.bind(this);
        this.state = {
            ID: "",
            password: "",
            current: 0,
            status: "process",
            loading: false,
            gift: "a"
        }
    }

    //发送数据
    postInfo(info, url) {
        this.setState({loading: true});
        return new Promise((resolve, reject)=> {
            let xhr = new XMLHttpRequest();
            xhr.open('post', url);
            xhr.onload = function (e) {
                if (this.status == 200) {
                    resolve(this.responseText);
                }
            };
            xhr.send(info);
        });
    };

    //校验
    stepBystep(verifi) {
        if (verifi.verifi) {
            this.setState({
                current: this.state.current + 1,
                status: this.state.current == 1 ? "finish" : "process",
                loading: false
            });
        } else {
            this.setState = ({
                status: "error",
                loading: true
            });
        }
    };

    makeSureIsVIP(syntheticEvent) {
        syntheticEvent.stopPropagation();
        syntheticEvent.preventDefault();

        const self = this;

        let postData = `ID=${self.state.ID}&password=${self.state.password}`;

        //控制器
        const TODO = async function () {
            let vetifi = await  self.postInfo(postData, "/vipActivities").then((data)=> {
                return JSON.parse(data);
            });
            await self.stepBystep(vetifi)
        };
        TODO();
    }

    handleChange(syntheticEvent) {
        this.setState({[syntheticEvent.target.name]: syntheticEvent.target.value});
    }

    doleGift(syntheticEvent) {
        syntheticEvent.stopPropagation();
        syntheticEvent.preventDefault();
        const self = this;
        let postData = `giftSort=${self.state.gift}`;
        //控制器
        const TODO = async function () {
            let vetifi = await  self.postInfo(postData, "/dolegift").then((data)=> {
                return JSON.parse(data);
            });
            await self.stepBystep(vetifi)
        };
        TODO();
    }

    render() {
        const formItemLayout = {
            labelCol: {span: 6},
            wrapperCol: {span: 8}
        };
        const step1 = (
            <Spin tip="Loading..." size="large" spinning={this.state.loading}>
                <Form horizontal onSubmit={this.makeSureIsVIP}>
                    <FormItem
                        {...formItemLayout}
                        label="会员帐号"
                    ><Input placeholder="1" name="ID" type="text" onChange={this.handleChange}/>
                    </FormItem>
                    <FormItem
                        {...formItemLayout}
                        label="密码"
                    >
                        <Input placeholder="1" name="password" type="password" onChange={this.handleChange}/>
                    </FormItem>
                    <FormItem wrapperCol={{ span: 9, offset: 4}}>
                        <Button type="primary" htmlType="submit">点击确认</Button>
                    </FormItem>
                </Form>
            </Spin>);

        const step2 = (<div>
            <RadioGroup onChange={this.handleChange} defaultValue="d">
                {  gifts.map(gift => {
                    return (<RadioButton name="gift" value={gift.type} key={gift.name}
                                         disable={gift.num==0}>{gift.name}</RadioButton>);
                })}
            </RadioGroup>

            <Button type="ghost" htmlType="submit" onClick={this.doleGift}>点击确认</Button>
        </div>);

        const step3 = "领取完成啦";

        return (<div className="memberArea">
            <Steps current={this.state.current} status={this.state.status}>
                {
                    steps.map((step)=> {
                        return ( <Step key={step.title} title={step.title} description={step.description}/>);
                    })
                }
            </Steps>
            <div className="steps-content">{
                (this.state.current == 0 && step1) || (this.state.current == 1 && step2) || ( this.state.current == 2 && step3)
            }
            </div>
        </div>);
    }
}

