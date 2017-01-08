import React,{Component,Proptypes} from  'react'
import { Form, InputNumber } from 'antd';
const FormItem = Form.Item;

export default class ExchangeTip extends Component {

    render() {
        return (

            <Form horizontal>
                <h2>您领取了{this.props.gifts}个iphone 7 Plus</h2>
                <FormItem
                    label="点击领取iphone 7 Plus"
                >
                    <InputNumber
                        onChange={this.props.todoWea.addGift}
                    />
                </FormItem>
            </Form>);
    }
}

